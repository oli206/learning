package model;

import java.io.File;
import java.io.IOException;

import org.apache.commons.io.FileUtils;

public class Converter {

	public static final String ENCODING_IBM = "Cp1047";
	
	public Converter()
	{
		
	}
	
	public String readFile(File _sourceFile) throws IOException
	{
		return FileUtils.readFileToString(_sourceFile, ENCODING_IBM);
	}
	
	public void convert(File _destinationFile, String _textToConvert) throws IOException
	{
		FileUtils.writeStringToFile(_destinationFile, _textToConvert, ENCODING_IBM);
	}
}
