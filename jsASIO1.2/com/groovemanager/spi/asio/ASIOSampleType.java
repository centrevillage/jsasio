/*
 * Created on 14.04.2004
 *
 */
package com.groovemanager.spi.asio;

/**
 * Collection of the ASIOSampleType constants
 * @author Manu Robledo
 *
 */
final class ASIOSampleType {
	/** */	final static int ASIOSTInt16MSB = ASIOSTInt16MSB();
	/** */ final static int ASIOSTInt16LSB = ASIOSTInt16LSB();
	/** used for 20 bits as well */final static int	ASIOSTInt24MSB = ASIOSTInt24MSB();
	/** used for 20 bits as well */ final static int ASIOSTInt24LSB = ASIOSTInt24LSB();
	/**  */ final static int ASIOSTInt32MSB = ASIOSTInt32MSB();
	/**  */ final static int ASIOSTInt32LSB = ASIOSTInt32LSB();
	/** 32 bit data with 16 bit alignment */ final static int ASIOSTInt32MSB16 = ASIOSTInt32MSB16();
	/** 32 bit data with 18 bit alignment */ final static int ASIOSTInt32LSB16 = ASIOSTInt32LSB16();
	/** 32 bit data with 18 bit alignment */ final static int ASIOSTInt32MSB18 = ASIOSTInt32MSB18();
	/** 32 bit data with 18 bit alignment */ final static int ASIOSTInt32LSB18 = ASIOSTInt32LSB18();
	/** 32 bit data with 20 bit alignment */ final static int ASIOSTInt32MSB20 = ASIOSTInt32MSB20();
	/** 32 bit data with 20 bit alignment */ final static int ASIOSTInt32LSB20 = ASIOSTInt32LSB20();
	/** 32 bit data with 24 bit alignment */ final static int ASIOSTInt32MSB24 = ASIOSTInt32MSB24();
	/** 32 bit data with 24 bit alignment */ final static int ASIOSTInt32LSB24 = ASIOSTInt32LSB24();
	/** IEEE 754 32 bit float */ final static int ASIOSTFloat32MSB = ASIOSTFloat32MSB();
	/** IEEE 754 32 bit float, as found on Intel x86 architecture */ final static int ASIOSTFloat32LSB = ASIOSTFloat32LSB();
	/** IEEE 754 64 bit double float */ final static int ASIOSTFloat64MSB = ASIOSTFloat64MSB();
	/** IEEE 754 64 bit double float, as found on Intel x86 architecture */ final static int ASIOSTFloat64LSB = ASIOSTFloat64LSB();
	
	private native static int ASIOSTInt16MSB();
	private native static int ASIOSTInt24MSB();
	private native static int ASIOSTInt32MSB();
	private native static int ASIOSTFloat32MSB();
	private native static int ASIOSTFloat64MSB();
	private native static int ASIOSTInt32MSB16();
	private native static int ASIOSTInt32MSB18();
	private native static int ASIOSTInt32MSB20();
	private native static int ASIOSTInt32MSB24();
	private native static int ASIOSTInt16LSB();
	private native static int ASIOSTInt24LSB();
	private native static int ASIOSTInt32LSB();
	private native static int ASIOSTFloat32LSB();
	private native static int ASIOSTFloat64LSB();
	private native static int ASIOSTInt32LSB16();
	private native static int ASIOSTInt32LSB18();
	private native static int ASIOSTInt32LSB20();
	private native static int ASIOSTInt32LSB24();
		
	private ASIOSampleType() {
	}
}