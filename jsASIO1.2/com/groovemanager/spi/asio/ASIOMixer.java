/*
 * Created on 18.04.2004
 *
 */
package com.groovemanager.spi.asio;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Control;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.Line;
import javax.sound.sampled.LineEvent;
import javax.sound.sampled.LineListener;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.Mixer;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.TargetDataLine;
import javax.sound.sampled.Control.Type;

import com.groovemanager.spi.asio.ASIOMixerProvider.ASIOMixerInfo;

/**
 * An ASIOMixer represents an ASIO driver
 * 
 * @author Manu Robledo
 */
public class ASIOMixer implements Mixer, ASIOListener {
	/**
	 * Because the ASIO driver can only say if he supports specific rates or
	 * not, we have to ask him before we can define the ASIOFormats it supports.
	 * These rates will be queried.
	 */
	final static double[] POSSIBLE_RATES = new double[] { 8000, 10000, 11025,
			12000, 16000, 20000, 22050, 24000, 30000, 32000, 40000, 44100,
			48000, 88200, 96000, 192000 };
	/**
	 * The info objects for getSourceLineInfo()
	 */
	private ASIOLineInfo[] sourceInfo;
	/**
	 * The info objects for getTargetLineInfo()
	 */
	private ASIOLineInfo[] targetInfo;
	/**
	 * The Mixer.Info corresponding to this Mixer
	 */
	private ASIOMixerProvider.ASIOMixerInfo mixerInfo;
	/**
	 * The ASIODriverInfo of the ASIO driver represented by this Mixer
	 */
	private ASIODriverInfo driverInfo;
	/**
	 * List of LineListeners
	 */
	private ArrayList<LineListener> listeners = new ArrayList<LineListener>();
	/**
	 * Status constants
	 */
	final static int UNLOADED = 0, LOADED = 1, INITIALIZED = 2, PREPARED = 3,
			RUNNING = 4;
	/**
	 * The current status of this ASIO driver
	 */
	private int status = UNLOADED;
	/**
	 * The lines associated to this mixer
	 */
	private HashMap<String, ASIODataLine> allLines;
	/**
	 * Lines that have to be opened simultaneously. Used with synchronize()
	 */
	private ArrayList<ASIODataLine> linesToSync = new ArrayList<ASIODataLine>();
	/**
	 * Lines that have been opened simultanously and must be started and stopped
	 * simultaneously.
	 */
	private ASIODataLine[] activeLines;
	/**
	 * ASIOBufferInfo objects used for ASIOPrepare()
	 */
	private ASIOBufferInfo[] asioBufferInfos;
	/**
	 * Sequence number from ASIOInit()-call
	 */
	private long initSeq = 0;
	/**
	 * Variable for getMicrosecondPosition() and getSamplePosition()
	 */
	private long lastTime = -1, lastSample = -1, nanosSinceOpen = -1,
			samplesSinceOpen = -1/* , stopTime = -1, stopFrame = -1 */;
	/**
	 * buffersize
	 */
	private int buffersize;
	/**
	 * List of ASIOListeners
	 */
	private ArrayList<ASIOListener> asioListeners = new ArrayList<ASIOListener>();
	/**
	 * The bufferIndex of the buffer half which should be filled right now and
	 * will be copied next to the asio buffers
	 */
	int bufferIndex;
	/**
	 * Temporary byte buffer for compy operations
	 */
	byte[] tempBuffer;
	// private ASIOMixerProvider asioMixerProvider;

	static {
		System.loadLibrary(ASIOMixerProvider.getLibName());
	}

	/**
	 * Initialize some variables in the native code for being able to accept
	 * callbacks from native code
	 * 
	 */
	private native void initVM();

	/**
	 * Free all references in the native code
	 * 
	 */
	private native void freeVM();

	/**
	 * Set the driver info of this ASIOMixer instance. Is called from the
	 * ASIOMixerProvider
	 * 
	 * @param driverInfo
	 */
	void setDriverInfo(ASIODriverInfo driverInfo) {
		this.driverInfo = driverInfo;
	}

	/**
	 * Get the name of the ASIO driver
	 * 
	 * @return The name of the ASIO driver
	 */
	String getName() {
		return mixerInfo.getName();
	}

	/**
	 * Get the version of the ASIO driver
	 * 
	 * @return The version of the ASIO driver
	 */
	int getDriverVersion() {
		if (driverInfo == null)
			return -1;

		return driverInfo.driverVersion();
	}

	/**
	 * Get the status of the ASIO driver
	 * 
	 * @return One of the status constants
	 */
	int getStatus() {
		return status;
	}

	/**
	 * create the lines that correspond to this Mixer. Should be called only
	 * once for one Mixer
	 */
	@SuppressWarnings("unchecked")
	private void createLines(boolean isInput) {
		if (allLines != null)
			return;
		allLines = new HashMap<String,ASIODataLine>();

//		if (!isInput) 
		{
			Line.Info[] infos = getSourceLineInfo();
			for (int i = 0; i < infos.length; i++) {
				ASIOLineInfo info = (ASIOLineInfo) infos[i];
				allLines.put(info.toString(), new ASIODataLine(this, info));
			}
		} //else
		{
			ASIOLineInfo[] infos = getTargetLineInfo();
			for (int i = 0; i < infos.length; i++) {
				ASIOLineInfo info = (ASIOLineInfo) infos[i];
				allLines.put(info.toString(), new ASIODataLine(this, info));
			}
		}
	}

	/**
	 * Open the ASIO Control Panel for this driver
	 */
	public void showControlPanel() {
		new Thread() {
			public void run() {
				long seq = 0;
				try {
					seq = ASIOInit();
					ASIOStaticFunctions.ASIOControlPanel();
				} catch (ASIOError e) {
					e.printStackTrace();
				}
				ASIOExit(seq);
			}
		}.start();
	}

	/**
	 * Constructs a new ASIOMixer
	 * 
	 * @param i
	 *            The ASIOInfo describing the ASIO driver
	 */
	ASIOMixer(ASIOMixerProvider.ASIOMixerInfo i) {
		this.mixerInfo = i;
	}

	/**
	 * 
	 * @see javax.sound.sampled.Mixer#getMixerInfo()
	 */
	public ASIOMixerInfo getMixerInfo() {
		return mixerInfo;
	}

	/**
	 * Set the status of this Mixer. Should only be called from
	 * ASIOMixerProvider
	 * 
	 * @param i
	 *            The new status
	 */
	void setStatus(int i) {
		if (status == i)
			return;
		int oldstatus = status;
		status = i;

		if (oldstatus < INITIALIZED && status >= INITIALIZED) {
			initVM();
			notifyListeners(new LineEvent(this, LineEvent.Type.OPEN, 0));
		} else if (oldstatus >= INITIALIZED && status < INITIALIZED) {
			freeVM();
			activeLines = null;
			notifyListeners(new LineEvent(this, LineEvent.Type.CLOSE, 0));
		}
		if (oldstatus < RUNNING && status == RUNNING)
			notifyListeners(new LineEvent(this, LineEvent.Type.START, 0));
		else if (oldstatus == RUNNING && status < RUNNING)
			notifyListeners(new LineEvent(this, LineEvent.Type.STOP, 0));
		status = i;
	}

	/**
	 * Try to initialize the ASIO driver. This is meeded for most inquiry
	 * methods. A sequence number is returned that must be passed to ASIOExit()
	 * after finishing the request. This sequence number is only valid if the
	 * driver was really initialized with this call. If it had been initialized
	 * before, the sequence number will be invalid. This mechanism gaurantees
	 * that ASIOExit only causes an unload of the driver if the caller is the
	 * one that initialized the driver.
	 * 
	 * @return A sequence number that should be used as an argument to
	 *         ASIOExit()
	 * @throws ASIOError
	 *             If neither input nor output is present (ASE_NotPresent),
	 *             other possible ASIOErrors are ASE_NoMemory and
	 *             ASE_HWMalfunction
	 */
	long ASIOInit() throws ASIOError {
		// if (asioMixerProvider == null) asioMixerProvider = new
		// ASIOMixerProvider();
		return ASIOMixerProvider.ASIOInit(this);
	}

	/**
	 * Calls ASIOCreateBuffers() and ASIO Init() for this Mixer if needed and
	 * returns a sequence number for the use with ASIOExit()
	 * 
	 * @param infos
	 *            Array of ASIOBufferInfos which will be filled in the call
	 * @param buffersize
	 *            The ASIO buffersize in sample frames
	 * @return A sequence number that should be used with ASIOExit()
	 * @throws ASIOError
	 *             If not enough memory is available for the buffers
	 *             (ASE_NoMemory), if no input/output is present
	 *             (ASE_NotPresent), if buffersize is not supported or one or
	 *             more of the ASIOBufferInfo elements contain invalid settings
	 *             (ASE_InvalidMode). Other possible value is ASE_HWMalfunction
	 */
	private long ASIOPrepare(ASIOBufferInfo[] infos, int buffersize)
			throws ASIOError {
		// if (asioMixerProvider == null) asioMixerProvider = new
		// ASIOMixerProvider();
		return ASIOMixerProvider.ASIOPrepare(this, infos, buffersize);
	}

	/**
	 * Calls ASIOStart(), ASIOCreateBuffers and ASIOInit() for this mixer if
	 * needed and returns a sequence number for the use with ASIOExit()
	 * 
	 * @param infos
	 *            Array of ASIOBufferInfos which will be filled in the call
	 * @param buffersize
	 *            The ASIO buffersize in sample frames
	 * @return A sequence number that should be used with ASIOExit()
	 * @throws ASIOError
	 *             If not enough memory is available for the buffers
	 *             (ASE_NoMemory), if no input/output is present
	 *             (ASE_NotPresent), if buffersize is not supported or one or
	 *             more of the ASIOBufferInfo elements contain invalid settings
	 *             (ASE_InvalidMode). Other possible value is ASE_HWMalfunction
	 */
	private long ASIOStart(ASIOBufferInfo[] infos, int buffersize)
			throws ASIOError {
		// if (asioMixerProvider == null) asioMixerProvider = new
		// ASIOMixerProvider();
		return ASIOMixerProvider.ASIOStart(this, infos, buffersize);
	}

	/**
	 * Calls ASIOStop() for this Mixer if needed
	 * 
	 * @return true if the call to ASIOStop() succeeded
	 * @throws ASIOError
	 *             If neither input nor output is present (ASE_NotPresent)
	 */
	private boolean ASIOStop() throws ASIOError {
		return ASIOMixerProvider.ASIOStop(this);
	}

	/**
	 * Calls ASIODisposeBuffers() and ASIOStop() for this Mixer if needed
	 * 
	 * @return true if the calls succeede, false otherwise
	 * @throws ASIOError
	 *             If neither input nor output is present (ASE_NotPresent), if
	 *             no buffers were ever created(ASE_InvalidMode)
	 */
	boolean ASIOUnPrepare() throws ASIOError {
		return ASIOMixerProvider.ASIOUnPrepare(this);
	}

	/**
	 * Calls ASIOExit(), ASIODisposeBuffers() and ASIOStop() for this Mixer if
	 * needed.
	 * 
	 * @param seq
	 *            A sequence number which should have been gotten from
	 *            ASIOInit(). Only if this sequence number is the one of the
	 *            call that initialized the driver, the driver will be unloaded.
	 */
	void ASIOExit(long seq) {
		// if (asioMixerProvider == null) asioMixerProvider = new
		// ASIOMixerProvider();
		ASIOMixerProvider.ASIOExit(this, seq);
		// asioMixerProvider = null;
	}

	/**
	 * Returns one ASIOLineInfo object for each mono output channel of this
	 * device. You can query the returned objects with ASIOLineInfo.getName().
	 * 
	 * @see javax.sound.sampled.Mixer#getSourceLineInfo()
	 */
	public ASIOLineInfo[] getSourceLineInfo() {
		if (sourceInfo != null)
			return sourceInfo;

		long seq = 0;
		try {
			seq = ASIOInit();
			if (seq == -1)
				return new ASIOLineInfo[0];

			int maxOutputs = ASIOStaticFunctions.ASIOGetOutputChannels();
			int minBufferSize = ASIOStaticFunctions.ASIOGetMinBufferSize();
			int maxBufferSize = ASIOStaticFunctions.ASIOGetMaxBufferSize();

			// One Line per Channel
			sourceInfo = new ASIOLineInfo[maxOutputs];

			for (int i = 0; i < sourceInfo.length; i++) {
				ASIOChannelInfo info = ASIOStaticFunctions.ASIOGetChannelinfo(
						i, false);
				AudioFormat[] formats = getAudioFormats(info);
				sourceInfo[i] = new ASIOLineInfo(this, info, formats,
						minBufferSize * getMinFrameSize(formats), maxBufferSize
								* getMaxFrameSize(formats));
			}

			ASIOExit(seq);
			return sourceInfo;

		} catch (ASIOError e) {
			e.printStackTrace();
			ASIOExit(seq);
			return new ASIOLineInfo[0];
		}
	}

	/**
	 * Get the minimum frame size out of some given AudioFormats. Needed for the
	 * conversion between ASIO�s buffersizes (in sample frames) and Java Sound�s
	 * buffersizes (in bytes)
	 * 
	 * @param formats
	 *            The formats to query for their frame size
	 * @return The minimum occuring frame size or AudioSystem.NOT_SPECIFIED
	 */
	static int getMinFrameSize(AudioFormat[] formats) {
		int min = AudioSystem.NOT_SPECIFIED;
		for (int i = 0; i < formats.length; i++) {
			if (min == AudioSystem.NOT_SPECIFIED)
				min = formats[i].getFrameSize();
			else if (formats[i].getFrameSize() != AudioSystem.NOT_SPECIFIED)
				min = Math.min(min, formats[i].getFrameSize());
		}
		return min;
	}

	/**
	 * Get the maximum frame size out of some given AudioFormats. Needed for the
	 * conversion between ASIO�s buffersizes (in sample frames) and Java Sound�s
	 * buffersizes (in bytes)
	 * 
	 * @param formats
	 *            The formats to query for their frame size
	 * @return The maximum occuring frame size or AudioSystem.NOT_SPECIFIED
	 */
	static int getMaxFrameSize(AudioFormat[] formats) {
		int max = AudioSystem.NOT_SPECIFIED;
		for (int i = 0; i < formats.length; i++) {
			if (max == AudioSystem.NOT_SPECIFIED)
				max = formats[i].getFrameSize();
			else
				max = Math.max(max, formats[i].getFrameSize());
		}
		return max;
	}

	/**
	 * Get all AudioFormats supported by an ASIODataLine. All supported sample
	 * rates will be taken into account as well as all possible numbers of
	 * channels. The rest is specified by the ASIOChannelInfo. If a line is the
	 * last one on a device (e.g. the 8th channel on an 8 channel device) it
	 * will only support mono formats. All other channels will support mono,
	 * stereo and all possible numbers of channels from their channel number up
	 * to the highest channel.
	 * 
	 * @param cInfo
	 *            The ASIOChannelInfo describing the channel
	 * @return An Array of AudioFormats supported by this channel
	 */
	@SuppressWarnings("unchecked")
	AudioFormat[] getAudioFormats(ASIOChannelInfo cInfo) {
		final int type = cInfo.type();
		ArrayList list = new ArrayList();
		int possibleChannels;
		long seq = 0;

		try {
			seq = ASIOInit();
			for (int i = 0; i < POSSIBLE_RATES.length; i++) {
				if (ASIOStaticFunctions.ASIOCanSampleRate(POSSIBLE_RATES[i]))
					list.add(new Double(POSSIBLE_RATES[i]));
			}
			if (cInfo.isInput())
				possibleChannels = ASIOStaticFunctions.ASIOGetInputChannels()
						- cInfo.channel();
			else
				possibleChannels = ASIOStaticFunctions.ASIOGetOutputChannels()
						- cInfo.channel();
			ASIOExit(seq);
		} catch (ASIOError e1) {
			e1.printStackTrace();
			ASIOExit(seq);
			return new AudioFormat[0];
		}

		AudioFormat[] formats;
		// If the Line supports all possible Rates, we assume, it supports any
		// (AudioSystem.NOT_SPECIFIED)
		if (list.size() == POSSIBLE_RATES.length) {
			formats = new AudioFormat[possibleChannels];
			for (int j = 0; j < possibleChannels; j++) {
				formats[j] = new ASIOAudioFormat(AudioSystem.NOT_SPECIFIED,
						type, j + 1);
			}
		} else {
			formats = new AudioFormat[list.size() * possibleChannels];
			int i = 0;
			for (Iterator iter = list.iterator(); iter.hasNext(); i++) {
				Double element = (Double) iter.next();
				for (int j = 0; j < possibleChannels; j++) {
					formats[possibleChannels * i + j] = new ASIOAudioFormat(
							element.floatValue(), type, j + 1);
				}
			}
		}

		return formats;
	}

	/**
	 * Returns one ASIOLineInfo object for each mono input channel of this
	 * device. You can query the returned objects with ASIOLineInfo.getName().
	 * 
	 * @see javax.sound.sampled.Mixer#getTargetLineInfo()
	 */
	public ASIOLineInfo[] getTargetLineInfo() {
		if (targetInfo != null)
			return targetInfo;

		long seq = 0;
		try {
			seq = ASIOInit();
			if (seq == -1)
				return new ASIOLineInfo[0];

			int maxInputs = ASIOStaticFunctions.ASIOGetInputChannels();
			int minBufferSize = ASIOStaticFunctions.ASIOGetMinBufferSize();
			int maxBufferSize = ASIOStaticFunctions.ASIOGetMaxBufferSize();

			// One line per mono channel
			targetInfo = new ASIOLineInfo[maxInputs];

			for (int i = 0; i < targetInfo.length; i++) {
				ASIOChannelInfo info = ASIOStaticFunctions.ASIOGetChannelinfo(
						i, true);
				AudioFormat[] formats = getAudioFormats(info);
				targetInfo[i] = new ASIOLineInfo(this, info, formats,
						minBufferSize * getMinFrameSize(formats), maxBufferSize
								* getMaxFrameSize(formats));
			}

			ASIOExit(seq);
			return targetInfo;

		} catch (ASIOError e) {
			e.printStackTrace();
			ASIOExit(seq);
			return new ASIOLineInfo[0];
		}
	}

	/**
	 * 
	 * @see javax.sound.sampled.Mixer#getSourceLineInfo(javax.sound.sampled.Line.Info)
	 */
	public Line.Info[] getSourceLineInfo(Line.Info info) {
		// Only SourceDataLine is supported
		if (!(SourceDataLine.class.isAssignableFrom(info.getLineClass())))
			return new Line.Info[0];

		// TODO ASIOLineInfo check
		else
			return getSourceLineInfo();
	}

	/**
	 * 
	 * @see javax.sound.sampled.Mixer#getTargetLineInfo(javax.sound.sampled.Line.Info)
	 */
	public Line.Info[] getTargetLineInfo(Line.Info info) {
		// Only TargetDataLine is supported
		if (!(TargetDataLine.class.isAssignableFrom(info.getLineClass())))
			return new Line.Info[0];

		// TODO ASIOLineInfo check
		else
			return getTargetLineInfo();
	}

	/**
	 * Get the minimum buffersize supported by this driver
	 * 
	 * @return The minimum buffer size in sample frames
	 * @throws ASIOError
	 *             if the driver could not be initialized
	 */
	int getMinBufferSize() throws ASIOError {
		long seq = ASIOInit();
		int result = ASIOStaticFunctions.ASIOGetMinBufferSize();
		ASIOExit(seq);
		return result;
	}

	/**
	 * Get the maximum buffersize supported by this driver
	 * 
	 * @return The maximum buffer size in sample frames
	 * @throws ASIOError
	 *             if the driver could not be initialized
	 */
	int getMaxBufferSize() throws ASIOError {
		long seq = ASIOInit();
		int result = ASIOStaticFunctions.ASIOGetMaxBufferSize();
		ASIOExit(seq);
		return result;
	}

	/**
	 * Indicate whether a Line object is belonging to this ASIOMixer instance
	 * 
	 * @param l
	 *            The Line to test
	 * @return true if this Line belongs to this Mixer, false otherwise
	 */
	private boolean isLineSupported(Line l) {
		if (!(l instanceof ASIODataLine))
			return false;
		return ((ASIODataLine) l).getMixer() == this;
	}

	/**
	 * 
	 * @see javax.sound.sampled.Mixer#isLineSupported(javax.sound.sampled.Line.Info)
	 */
	public boolean isLineSupported(Line.Info info) {
		DataLine.Info i;
		if (!(info instanceof DataLine.Info)) {
			return false;
		}

		if (!ASIOMixerProvider.isFullCheck())
			return true;

		i = (DataLine.Info) info;

		Line.Info[] infos;
		if (SourceDataLine.class.isAssignableFrom(i.getLineClass()))
			infos = getSourceLineInfo();
		else if (TargetDataLine.class.isAssignableFrom(i.getLineClass()))
			infos = getTargetLineInfo();
		else {
			return false;
		}

		try {
			if (i.getMaxBufferSize() < getMinBufferSize()
					* getMinFrameSize(i.getFormats()))
				return false;
			if (i.getMinBufferSize() > getMaxBufferSize()
					* getMaxFrameSize(i.getFormats()))
				return false;
		} catch (ASIOError e1) {
			e1.printStackTrace();
			return false;
		}

		AudioFormat[] formats = i.getFormats();
		for (int j = 0; j < formats.length; j++) {
			for (int k = 0; k < infos.length; k++) {
				if (((DataLine.Info) infos[k]).isFormatSupported(formats[j])) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * 
	 * @see javax.sound.sampled.Mixer#getLine(javax.sound.sampled.Line.Info)
	 */
	public ASIODataLine getLine(Line.Info info) throws LineUnavailableException {
		if (!isLineSupported(info))
			throw new LineUnavailableException("Line not supported.");

		boolean isInput;
		if (SourceDataLine.class.isAssignableFrom(info.getLineClass()))
			isInput = false;
		else if (TargetDataLine.class.isAssignableFrom(info.getLineClass()))
			isInput = true;
		else
			throw new LineUnavailableException("Line not supported.");

		if (allLines == null)
			createLines(isInput);

		if (info instanceof ASIOLineInfo) {
			ASIOLineInfo asioInfo = (ASIOLineInfo) info;
			if (allLines.containsKey(asioInfo.toString()))
				return allLines.get(asioInfo.toString());
		}

		/*
		 * boolean isInput;
		 * if(SourceDataLine.class.isAssignableFrom(info.getLineClass()))
		 * isInput = false; else
		 * if(TargetDataLine.class.isAssignableFrom(info.getLineClass()))
		 * isInput = true; else throw new LineUnavailableException("Line not
		 * supported.");
		 */

		for (Iterator<String> iter = allLines.keySet().iterator(); iter
				.hasNext();) {
			ASIODataLine line = allLines.get(iter.next());
			ASIOLineInfo lineInfo = line.getLineInfo();
			if (lineInfo.getChannelInfo().isInput() == isInput) {
				AudioFormat[] formats = ((DataLine.Info) info).getFormats();
				for (int i = 0; i < formats.length; i++) {
					if (lineInfo.isFormatSupported(formats[i]))
						return line;
				}
			}
		}
		throw new LineUnavailableException("Line not supported.");
	}

	/**
	 * 
	 * @see javax.sound.sampled.Mixer#getMaxLines(javax.sound.sampled.Line.Info)
	 */
	public int getMaxLines(Line.Info info) {
		if (!isLineSupported(info))
			return 0;
		else
			return 1;
	}

	/**
	 * 
	 * @see javax.sound.sampled.Mixer#getSourceLines()
	 */
	public ASIODataLine[] getSourceLines() {
		ASIOLineInfo[] infos = getSourceLineInfo();
		ASIODataLine[] result = new ASIODataLine[infos.length];
		if (allLines == null) this.createLines(false);
		for (int i = 0; i < infos.length; i++) {
			ASIOLineInfo info = infos[i];
			String info1 = info.toString();
			result[i] = allLines.get(info1);
			if (result[i]==null) System.err.println("Error: "+info1+"was not found");
		}
		return result;
	}

	/**
	 * 
	 * @see javax.sound.sampled.Mixer#getTargetLines()
	 */
	public ASIODataLine[] getTargetLines() {
		ASIOLineInfo[] infos = getTargetLineInfo();
		ASIODataLine[] result = new ASIODataLine[infos.length];
		if (allLines == null) this.createLines(true);

		for (int i = 0; i < infos.length; i++) {
			ASIOLineInfo info = infos[i];
			result[i] = allLines.get(info.toString());
		}
		return result;
	}

	/**
	 * ASIO automatically synchronizes all DataLines on one Mixer. But if you
	 * want to use more than one line simultaneosly on an ASIOMixer, you will
	 * have to call this Method before opening a line. Otherwise trying to open
	 * a Line when another one is already open on this Mixer would result in a
	 * LineUnavailableException
	 * 
	 * @see javax.sound.sampled.Mixer#synchronize(javax.sound.sampled.Line[],
	 *      boolean)
	 */
	public void synchronize(Line[] lines, boolean maintainSync) {
		if (!isSynchronizationSupported(lines, maintainSync))
			return;
		for (int i = 0; i < lines.length; i++) {
			if (!linesToSync.contains(lines[i]))
				if (lines[i] instanceof ASIODataLine) {
					ASIODataLine asioLine = (ASIODataLine) lines[i];
					linesToSync.add(asioLine);
				} else
					System.err.println("Error: ASIOMixer.synchronize - Line "
							+ i + " is not an ASIO Line!");
		}
	}

	/**
	 * 
	 * @see javax.sound.sampled.Mixer#unsynchronize(javax.sound.sampled.Line[])
	 */
	public void unsynchronize(Line[] lines) {
		for (int i = 0; i < lines.length; i++) {
			linesToSync.remove(lines[i]);
		}
	}

	/**
	 * Get the number of input channels on this device
	 * 
	 * @return The number of input channels or 0 if the ASIO driver can not be
	 *         initialized
	 */
	@SuppressWarnings("unused")
	private int getMaxInChannels() {
		try {
			long seq = ASIOInit();
			int res = ASIOStaticFunctions.ASIOGetInputChannels();
			ASIOExit(seq);
			return res;
		} catch (ASIOError e) {
			return 0;
		}
	}

	/**
	 * Get the number of output channels on this device
	 * 
	 * @return The number of input channels or 0 if the ASIO driver can not be
	 *         initialized
	 */
	@SuppressWarnings("unused")
	private int getMaxOutChannels() {
		try {
			long seq = ASIOInit();
			int res = ASIOStaticFunctions.ASIOGetOutputChannels();
			ASIOExit(seq);
			return res;
		} catch (ASIOError e) {
			return 0;
		}
	}

	/**
	 * Synchronization is supported for all Lines of this Mixer.
	 * 
	 * @see javax.sound.sampled.Mixer#isSynchronizationSupported(javax.sound.sampled.Line[],
	 *      boolean)
	 */
	public boolean isSynchronizationSupported(Line[] lines, boolean maintainSync) {
		for (int i = 0; i < lines.length; i++) {
			if (!isLineSupported(lines[i]))
				return false;
		}
		return true;
	}

	/**
	 * 
	 * @see javax.sound.sampled.Line#getLineInfo()
	 */
	public Line.Info getLineInfo() {
		return new Line.Info(Mixer.class);
	}

	/**
	 * 
	 * @see javax.sound.sampled.Line#open()
	 */
	public void open() throws LineUnavailableException {
		try {
			initSeq = ASIOInit();
		} catch (ASIOError e) {
			throw new LineUnavailableException(e.getMessage());
		}
	}

	/**
	 * 
	 * @see javax.sound.sampled.Line#close()
	 */
	public void close() {
		close(initSeq);
		initSeq = 0;
	}

	/**
	 * Closes this Mixer only if the sequence number matches the one which was
	 * generated by ASIOInit()
	 * 
	 * @param seq
	 *            The sequence number
	 */
	void close(long seq) {
		ASIOExit(seq);
		if (activeLines != null)
			for (int i = 0; i < activeLines.length; i++) {
				activeLines[i].closed();
			}
		activeLines = null;
	}

	/**
	 * 
	 * @see javax.sound.sampled.Line#isOpen()
	 */
	public boolean isOpen() {
		return status >= INITIALIZED;
	}

	/**
	 * No Controls are supported by ASIOMixer.
	 * 
	 * @see javax.sound.sampled.Line#getControls()
	 */
	public Control[] getControls() {
		return new Control[0];
	}

	/**
	 * No Controls are supported by ASIOMixer.
	 * 
	 * @see javax.sound.sampled.Line#isControlSupported(javax.sound.sampled.Control.Type)
	 */
	public boolean isControlSupported(Type control) {
		return false;
	}

	/**
	 * No Controls are supported by ASIOMixer.
	 * 
	 * @see javax.sound.sampled.Line#getControl(javax.sound.sampled.Control.Type)
	 */
	public Control getControl(Type control) {
		return null;
	}

	/**
	 * 
	 * @see javax.sound.sampled.Line#addLineListener(javax.sound.sampled.LineListener)
	 */
	@SuppressWarnings("unchecked")
	public void addLineListener(LineListener listener) {
		listeners.add(listener);
	}

	/**
	 * 
	 * @see javax.sound.sampled.Line#removeLineListener(javax.sound.sampled.LineListener)
	 */
	public void removeLineListener(LineListener listener) {
		listeners.remove(listener);
	}

	/**
	 * Notify the LineListeners of a LineEvent
	 * 
	 * @param e
	 *            The LineEvent
	 */
	private void notifyListeners(LineEvent e) {
		for (Iterator<LineListener> iter = listeners.iterator(); iter.hasNext();) {
			LineListener listener = iter.next();
			listener.update(e);
		}
	}

	/**
	 * A request from an ASIOdataLine to be opened with default buffersize
	 * 
	 * @param line
	 *            The ASIODataLine the askes to be opened
	 * @throws LineUnavailableException
	 */
	void openLine(ASIODataLine line) throws LineUnavailableException {
		openLine(line, getPreferredBufferSize());
	}

	/**
	 * Get the preferreed buffersize of the ASIO driver
	 * 
	 * @return The preferred buffer size in sample frames or 0 if the driver
	 *         could not be initialized
	 */
	int getPreferredBufferSize() {
		long seq = 0;
		try {
			seq = ASIOInit();
			int buffersize = ASIOStaticFunctions.ASIOGetPreferredBufferSize();
			ASIOExit(seq);
			return buffersize;
		} catch (ASIOError e) {
			ASIOExit(seq);
			return 0;
		}
	}

	/**
	 * A request from an ASIODataLine to be opened with the given buffersize
	 * 
	 * @param line
	 *            The ASIODataLine which requests to be opened
	 * @param buffersize
	 *            The requested buffersize in sample frames
	 * @throws LineUnavailableException
	 *             If other lines are already open on this Mixer or if the
	 *             driver could not be initialized
	 */
	void openLine(ASIODataLine line, int buffersize)
			throws LineUnavailableException {
		if (line.isOpen())
			return;
		//TODO:Pr�fung ob das funktioniert
		if (status >= PREPARED)
			throw new LineUnavailableException(
					"ASIO is already activated with another Line.");
		ASIODataLine[] lines;

		if (linesToSync.contains(line)) {
			lines = new ASIODataLine[linesToSync.size()];
			int i = 0;
			if (ASIOMixerProvider.isFullCheck()) {
				for (Iterator<ASIODataLine> iter = linesToSync.iterator(); iter
						.hasNext(); i++) {
					lines[i] = (ASIODataLine) iter.next();
					if (!((DataLine.Info) lines[i].getLineInfo())
							.isFormatSupported(lines[i].getFormat()))
						throw new LineUnavailableException("Format "
								+ lines[i].getFormat() + " not supported.");
				}
			} else
				for (Iterator<ASIODataLine> iter = linesToSync.iterator(); iter
						.hasNext(); i++) {
					lines[i] = iter.next();
				}
		} else {
			if (ASIOMixerProvider.isFullCheck()) {
				DataLine.Info info = (DataLine.Info) line.getLineInfo();
				if (!info.isFormatSupported(line.getFormat()))
					throw new LineUnavailableException("Format "
							+ line.getFormat() + " not supported.");
			}
			lines = new ASIODataLine[] { line };
		}

		prepare(lines, buffersize / 2);
	}

	/**
	 * A request from an ASIODataLine to be started
	 * 
	 * @param line
	 *            The ASIODataLine which requests to be started
	 */
	void startLine(ASIODataLine line) {
		if (status < PREPARED)
			try {
				openLine(line);
			} catch (LineUnavailableException e) {
				e.printStackTrace();
				return;
			}

		if (activeLines == null)
			return;
		for (int i = 0; i < activeLines.length; i++) {
			activeLines[i].started();
		}

		lastSample = -1;
		lastTime = -1;
		// ASIO expects the second buffer half to be filled before
		// ASIOStart() is called, so lets hope, the application has
		// written enough already
		// copyThread.copyNow(1, true);
		bufferSwitchTimeInfo(null, 1, true, true);

		try {
			ASIOStart(asioBufferInfos, buffersize);
		} catch (ASIOError e1) {
			e1.printStackTrace();
		}
	}

	/**
	 * Prepare all Lines, buffers etc. for being able to start.
	 * 
	 * @param lines
	 *            The ASIODataLines which should be synchronized
	 * @param buffersize
	 *            The buffersize in sample frames
	 */
	private void prepare(ASIODataLine[] lines, int buffersize) {
		int channelcount = 0;
		// bufferIndex = 1;
		for (int i = 0; i < lines.length; i++) {
			channelcount += lines[i].getFormat().getChannels();
		}
		ASIOBufferInfo[] infos = new ASIOBufferInfo[channelcount];
		channelcount = 0;
		for (int i = 0; i < lines.length; i++) {
			int channels = lines[i].getFormat().getChannels();
			for (int j = 0; j < channels; j++) {
				infos[channelcount + j] = new ASIOBufferInfo();
				infos[channelcount + j]
						.setChannelNum(lines[i].getChannel() + j);
				infos[channelcount + j].setIsInput(lines[i].isInput());
			}
			channelcount += channels;
		}

		try {
			if (ASIOMixerProvider.isFullCheck()) {
				int min = getMinBufferSize();
				int max = getMaxBufferSize();
				int gran = getBufferSizeGranularity();
				buffersize = Math.max(buffersize, min);
				buffersize = Math.min(buffersize, max);
				if (gran != 0)
					buffersize -= buffersize % gran;
			} else {
				int gran = 32;
				buffersize -= buffersize % gran;
			}

			long seq = 0;
			seq = ASIOPrepare(infos, buffersize);
			// if(copyThread != null) copyThread.end = true;
			nanosSinceOpen = -1;
			samplesSinceOpen = -1;
			lastSample = -1;
			lastTime = -1;
			asioBufferInfos = infos;
			this.buffersize = buffersize;

			for (int i = 0; i < lines.length; i++) {
				int channels = lines[i].getFormat().getChannels();
				ByteBuffer[][] buffers = new ByteBuffer[channels][];
				ASIOBufferInfo[] bInfos = new ASIOBufferInfo[channels];
				for (int j = 0; j < channels; j++) {
					buffers[j] = ASIOStaticFunctions.createBuffers(
							infos[i + j], buffersize
									* lines[i].getFormat().getFrameSize()
									/ lines[i].channels);
					bInfos[j] = infos[i + j];
				}
				lines[i].opened(seq, buffers);
			}
			activeLines = lines;
			/*
			 * copyThread = new CopyBuffersThread(activeLines);
			 * copyThread.start();
			 */
		} catch (ASIOError e) {
			e.printStackTrace();
		}
	}

	/**
	 * Get the granularity of the driver�s buffersize in sample frames
	 * 
	 * @return the granularity at which buffer sizes may differ. Usually, the
	 *         buffer size will be a power of 2; in this case, granularity will
	 *         hold -1 on return, signaling possible buffer sizes starting from
	 *         minSize, increased in powers of 2 up to maxSize.
	 */
	private int getBufferSizeGranularity() {
		long asioSequenceNumber = 0;
		try {
			asioSequenceNumber = ASIOInit();
			int gran = ASIOStaticFunctions.ASIOGetBufferSizeGranularity();
			ASIOExit(asioSequenceNumber);
			return gran;
			// }
		} catch (ASIOError e) {
			ASIOExit(asioSequenceNumber);
			System.err.print(e.getMessage());
			return 0;
		}
	}

	/**
	 * Request from an ASIODataLine to be stopped
	 * 
	 * @param line
	 *            The ASIODataLine which requested to be stopped
	 */
	void stopLine(ASIODataLine line) {
		try {
			ASIOStop();
		} catch (ASIOError e) {
			e.printStackTrace();
		}
		if (activeLines == null)
			line.stopped();
		else
			for (int i = 0; i < activeLines.length; i++) {
				activeLines[i].stopped();
			}
	}

	/**
	 * Is used for DataLine.getMicrosencondposition()
	 * 
	 * @return The number of microseconds that this line has been running since
	 *         it was opened
	 */
	long getMicrosecondPosition() {
		if (status < PREPARED)
			return 0;
		if (nanosSinceOpen == -1)
			return 0;
		if (lastTime == -1)
			return nanosSinceOpen;
		long time = ASIOStaticFunctions.getSystemTime();
		return (time - lastTime + nanosSinceOpen) / 1000;
	}

	/**
	 * This method is called from the native bufferSwitchTimeInfo callback
	 * 
	 * @param asioTime
	 *            Pointer to an ASIOTime structure
	 * @param index
	 *            The buffer half index that should be processed
	 * @param directProcess
	 *            Indicates whether this method should return immediately or
	 *            should process the buffer contents immediately
	 */
	void jBufferSwitchTimeInfo(long asioTime, int index, boolean directProcess) {
		Thread.currentThread().setPriority(Thread.MAX_PRIORITY);
		ASIOTime time = null; // new ASIOTime(asioTime);
		bufferSwitchTimeInfo(time, index, directProcess, false);
	}

	/**
	 * ASIO�s bufferSwitchTimeInfo() callback
	 * 
	 * @param time
	 *            ASIOTime object that represents the time and sample position
	 *            at which this call was launched
	 * @param index
	 *            The buffer half index that should be processed
	 * @param processNow
	 *            Indicates whether this method should return immediately or
	 *            should process the buffer contents immediately
	 * @param internal
	 *            Indicates if this call has come from the ASIO driver (false)
	 *            or as an internal call from this Mixer (true)
	 */
	void bufferSwitchTimeInfo(ASIOTime time, int index, boolean processNow,
			boolean internal) {
		// If this happens: panic!!
		if ((getStatus() < RUNNING && !internal) || activeLines == null) {
			System.out
					.println("bufferSwitchTimeInfo() called in wrong state!!");
			ASIOMixerProvider.freeAll();
			return;
		}

		if (!internal && time != null) {
			long t;
			if ((time.timeInfo.flags() & AsioTimeInfo.FLAG_kSystemTimeValid) > 0) {
				t = ASIOStaticFunctions.getSystemTime();
				if (nanosSinceOpen == -1)
					nanosSinceOpen = 0;
				else if (lastTime != -1)
					nanosSinceOpen += t - lastTime;
				lastTime = t;
			}
			if ((time.timeInfo.flags() & AsioTimeInfo.FLAG_kSamplePositionValid) > 0) {
				t = (long) time.timeInfo.samplePosition().doubleValue();
				if (samplesSinceOpen == -1)
					samplesSinceOpen = 0;
				else if (lastSample != -1)
					samplesSinceOpen += t - lastSample;
				lastSample = (long) t;
			}
		}
		// boolean wait = false;
		synchronized (this) {
			if (index == 0)
				bufferIndex = 1;
			else
				bufferIndex = 0;
			for (int i = 0; i < activeLines.length; i++) {
				ASIODataLine line = activeLines[i];
				for (int j = 0; j < line.channels; j++) {
					// Copy the content to the desired buffer
					// line.internalBuffers[j][index].rewind();
					line.asioBuffers[j][index].rewind();
					// if(line.isInput())
					// line.internalBuffers[j][index].put(line.asioBuffers[j][index]);
					int length = line.asioBuffers[j][index].capacity();
					if (tempBuffer == null || tempBuffer.length < length)
						tempBuffer = new byte[length];
					if (line.isInput()) {
						line.asioBuffers[j][index].get(tempBuffer, 0, length);
						line.ringBuffers[j].write(tempBuffer, 0, length, true);
					} else {
						line.ringBuffers[j].read(tempBuffer, 0, length, true);
						line.asioBuffers[j][index].put(tempBuffer, 0, length);
					}

					// Rewind the next buffer so that it can be written again
					// line.internalBuffers[j][bufferIndex].rewind();
				}
			}
		}
	}

	/**
	 * Request from an ASIODataLine to be closed
	 * 
	 * @param line
	 *            The ASIODataLine which requests to be closed
	 * @param seq
	 *            The sequence number with which this line was opened
	 */
	void closeLine(ASIODataLine line, long seq) {
		close(seq);
		line.closed();
	}

	/**
	 * Get the current sample rate of this ASIO driver
	 * 
	 * @return The current sample rate of this ASIO driver
	 */
	double getSampleRate() {
		long seq = 0;
		try {
			seq = ASIOInit();
			double rate = ASIOStaticFunctions.ASIOGetSampleRate();
			ASIOExit(seq);
			return rate;
		} catch (ASIOError e) {
			ASIOExit(seq);
			return AudioSystem.NOT_SPECIFIED;
		}
	}

	/**
	 * Is used by ASIODataLine.getFramePosition()
	 * 
	 * @return The number of sampleFrames played back or recorded since the Line
	 *         was opened
	 */
	long getSamplePosition() {
		if (status < PREPARED)
			return 0;
		if (samplesSinceOpen == -1)
			return 0;
		if (lastSample == -1)
			return samplesSinceOpen;
		long time = ASIOStaticFunctions.getSystemTime();
		long elapsed = time - lastTime;
		double sampleRate = getSampleRate();
		if (sampleRate == AudioSystem.NOT_SPECIFIED)
			sampleRate = 44100;
		return samplesSinceOpen + (long) (elapsed / 1000000000 * sampleRate);
	}

	/**
	 * Adds an ASIOListener that will be notified of callbacks from this ASIO
	 * driver
	 * 
	 * @param listener
	 *            The ASIOlistener to be added
	 */
	@SuppressWarnings("unchecked")
	public void addASIOListener(ASIOListener listener) {
		asioListeners.add(listener);
	}

	/**
	 * Removes a registeres ASIOListener
	 * 
	 * @param listener
	 *            The ASIOListener to be removed
	 */
	public void removeASIOListener(ASIOListener listener) {
		asioListeners.remove(listener);
	}

	/**
	 * 
	 * @see com.groovemanager.spi.asio.ASIOListener#sampleRateChanged(com.groovemanager.spi.asio.ASIOMixer,
	 *      double)
	 */
	public void sampleRateChanged(ASIOMixer mixer, double newRate) {
		if (mixer != this)
			return;
		for (Iterator<ASIOListener> iter = asioListeners.iterator(); iter
				.hasNext();) {
			ASIOListener listener = iter.next();
			listener.sampleRateChanged(this, newRate);
		}
	}

	/**
	 * 
	 * @see com.groovemanager.spi.asio.ASIOListener#resetRequest(com.groovemanager.spi.asio.ASIOMixer)
	 */
	public void resetRequest(ASIOMixer mixer) {
		if (mixer != this)
			return;
		for (Iterator<ASIOListener> iter = asioListeners.iterator(); iter
				.hasNext();) {
			ASIOListener listener = iter.next();
			listener.resetRequest(this);
		}
	}

	/**
	 * 
	 * @see com.groovemanager.spi.asio.ASIOListener#supportsBufferSizeChange()
	 */
	public boolean supportsBufferSizeChange() {
		for (Iterator<ASIOListener> iter = asioListeners.iterator(); iter
				.hasNext();) {
			ASIOListener listener = iter.next();
			if (listener.supportsBufferSizeChange())
				return true;
		}
		return false;
	}

	/**
	 * 
	 * @see com.groovemanager.spi.asio.ASIOListener#bufferSizeChanged(com.groovemanager.spi.asio.ASIOMixer,
	 *      int)
	 */
	public boolean bufferSizeChanged(ASIOMixer mixer, int newSize) {
		if (mixer != this)
			return false;
		boolean accepted = false;
		for (Iterator<ASIOListener> iter = asioListeners.iterator(); iter
				.hasNext();) {
			ASIOListener listener = iter.next();
			if (listener.supportsBufferSizeChange())
				accepted = accepted
						|| listener.bufferSizeChanged(this, newSize);
		}
		return accepted;
	}

	/**
	 * 
	 * @see com.groovemanager.spi.asio.ASIOListener#supportsResyncRequest()
	 */
	public boolean supportsResyncRequest() {
		for (Iterator<ASIOListener> iter = asioListeners.iterator(); iter
				.hasNext();) {
			ASIOListener listener = iter.next();
			if (listener.supportsResyncRequest())
				return true;
		}
		return false;
	}

	/**
	 * 
	 * @see com.groovemanager.spi.asio.ASIOListener#resyncRequest(com.groovemanager.spi.asio.ASIOMixer)
	 */
	public boolean resyncRequest(ASIOMixer mixer) {
		if (mixer != this)
			return false;
		boolean accepted = false;
		for (Iterator<ASIOListener> iter = asioListeners.iterator(); iter
				.hasNext();) {
			ASIOListener listener = iter.next();
			if (listener.supportsResyncRequest())
				accepted = accepted || listener.resyncRequest(this);
		}
		return accepted;
	}

	public boolean supportsLatenciesChanged() {
		for (Iterator<ASIOListener> iter = asioListeners.iterator(); iter
				.hasNext();) {
			ASIOListener listener = iter.next();
			if (listener.supportsLatenciesChanged())
				return true;
		}
		return false;
	}

	/**
	 * 
	 * @see com.groovemanager.spi.asio.ASIOListener#latenciesChanged(com.groovemanager.spi.asio.ASIOMixer)
	 */
	public boolean latenciesChanged(ASIOMixer mixer) {
		if (mixer != this)
			return false;
		boolean accepted = false;
		for (Iterator<ASIOListener> iter = asioListeners.iterator(); iter
				.hasNext();) {
			ASIOListener listener = iter.next();
			if (listener.supportsLatenciesChanged())
				accepted = accepted || listener.latenciesChanged(this);
		}
		return accepted;
	}

	/**
	 * Get the current input Latency in sample frames
	 * 
	 * @return The current input latency in sample frames
	 */
	public int getInputLatency() {
		long seq = 0;
		try {
			seq = ASIOInit();
			int l = ASIOStaticFunctions.ASIOGetInputLatency();
			ASIOExit(seq);
			return l;
		} catch (ASIOError e) {
			e.printStackTrace();
			ASIOExit(seq);
		}
		return AudioSystem.NOT_SPECIFIED;
	}

	/**
	 * Get the current output Latency in sample frames
	 * 
	 * @return The current output latency in sample frames
	 */
	public int getOutputLatency() {
		long seq = 0;
		try {
			seq = ASIOInit();
			int l = ASIOStaticFunctions.ASIOGetOutputLatency();
			ASIOExit(seq);
			return l;
		} catch (ASIOError e) {
			e.printStackTrace();
			ASIOExit(seq);
		}
		return AudioSystem.NOT_SPECIFIED;
	}
}