package it.smartdatanet.smartlab.calibration;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 
 * An example program that uses the calibration library.
 * The program takes a CSV file name as argument and initializes
 * the calibration class using this file.
 * The second argument is a value (from the sensor). This value
 * will be corrected and printed on standard output.
 * At the end the program will print the CSV representation
 * of the function used to correct the value.
 * The idea is to use this CSV file to export the correction
 * capabilities to other programs (Linux gateways or Arduino
 * smart objects for example).
 * 
 *
 */
public class Example {
	public static double[] toDouble(ArrayList<Double> aDoubles)
	{
	    double[] doubles = new double[aDoubles.size()];
	    Iterator<Double> iterator = aDoubles.iterator();
	    int i=0;
	    while(iterator.hasNext())
	    {
	        doubles[i] = iterator.next().doubleValue();
	        i++;
	    }
	    return doubles;
	}
	
	public static void main(String[] args) {
		if (args.length != 2) {
			System.out.println("Usage: <csvfile> <value>");
		} else {
			try {
				/* A basic CSV  parser. The CSV is expected to have only two
				 * columns.
				 * Each line contains a point of calibration having
				 * the expected measure and the one acquired by the sensor.
				 * 
				 * <reference value> , <observed value>
				 * 
				 */
				System.out.println("Reading CSV file...");
				
				FileInputStream fis;
				fis = new FileInputStream(args[0]);
				BufferedReader br = new BufferedReader(new InputStreamReader(fis));
				String line;
				ArrayList<Double> referenceValues = new ArrayList<Double>();
				ArrayList<Double> observedValues = new ArrayList<Double>();
				
				while ((line = br.readLine()) != null) {
					String[] parts = line.split("\\s*,\\s*");
					referenceValues.add(Double.parseDouble(parts[0]));
					observedValues.add(Double.parseDouble(parts[1]));
				}
				
				br.close();
				
				/* Setup the calibrator */
				System.out.println("Setting-up calibration...");
				Calibrator calibrator = new SplineCalibrator();
				if (calibrator.setup(toDouble(observedValues),toDouble(referenceValues))) {
					/* Print the corrected value and CSV representation
					 * of the interpolation function. */
					double val = Double.parseDouble(args[1]);
					double corrected = calibrator.correct(val);
					System.out.println(String.format("%f -> %f", val, corrected));
					System.out.println("CSV:");
					System.out.println(calibrator.toString());					
				}
				
				
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

}
