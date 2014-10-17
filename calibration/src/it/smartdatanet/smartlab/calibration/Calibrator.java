package it.smartdatanet.smartlab.calibration;

/**
 * An interface to a generic calibrator.
 * 
 * Example usage:
 * 
 * <code>
 * Calibrator calibrator = new SplineCalibrator();
 * double observed[] = [1, 1.5, 12, 13];
 * double reference[] = [0.5, 2, 13, 15];
 * if (calibrator.setup(observed, reference)) {
 *     double measure = 3.4;
 *     double correctedMeasure = calibrator.correct(measure);
 * }
 * </code>
 * 
 */
public interface Calibrator {

	/**
	 * The method computes an interpolation function starting from
	 * the reference and related observed values provided by the sensor.
	 * The observed values should be ordered in ascending order and 
	 * the the dimensions of observed/reference arrays should be
	 * exactly the same (values are paired).
	 * 
	 * @param observedValues the array with the observed values
	 * @param referenceValues the array of expected reference values
	 * @return True if the calibrator has found the interpolation function
	 */
	boolean setup(double[] observedValues, double[] referenceValues);
	
	/**
	 * Using the in interpolation function computed by
	 * @see yucca.calibration.Calibrator#setup() method this method will
	 * return the corrected from the specified measure observed.
	 * 
	 * @param measure the measure observed by the sensor
	 * @return the corrected value
	 */
	double correct(double measure);
	
}
