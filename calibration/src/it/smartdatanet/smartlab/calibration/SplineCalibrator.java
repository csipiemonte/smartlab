package it.smartdatanet.smartlab.calibration;

import org.apache.commons.math3.analysis.UnivariateFunction;
import org.apache.commons.math3.analysis.interpolation.SplineInterpolator;
import org.apache.commons.math3.analysis.interpolation.UnivariateInterpolator;
import org.apache.commons.math3.analysis.polynomials.PolynomialFunction;
import org.apache.commons.math3.analysis.polynomials.PolynomialSplineFunction;
import org.apache.commons.math3.exception.DimensionMismatchException;
import org.apache.commons.math3.exception.MathIllegalArgumentException;

/**
 * Implements a Calibrator using Apache Commons Math3 SplineInterpolator,
 * to build and evaluate the interpolation function.
 *
 */
public class SplineCalibrator implements Calibrator {

	private UnivariateFunction function;
		
	@Override
	public boolean setup(double[] observedValues, double[] referenceValues) {
		UnivariateInterpolator interpolator = new SplineInterpolator();
		boolean setupSuccessful = false;
		
		try {
			this.function = interpolator.interpolate(observedValues, referenceValues);
			setupSuccessful = true;
		}
		catch (DimensionMismatchException e) {
			
		}
		catch (MathIllegalArgumentException e) {
			
		}
		
		return setupSuccessful;
	}

	@Override
	public double correct(double measure) {
		
		/* The evaluation is valid only in the range defined by
		 * the observed values provided in setup() method as described
		 * by Apache Common Math documentation.
		 * 
		 * Instead of generating an error/exception, this method simply
		 * returns the untouched value if it is out of range.
		 */
		
		double correctedValue = measure;
		
		try {
			correctedValue = this.function.value(measure);
		}
		catch (IllegalArgumentException e) {
			
		}
		
		return correctedValue;
	}

	@Override
	public String toString() {
		/* Use the toString() override to return the CSV representation
		 * of the spline function.
		 * 
		 * The SplineInterpolator provides 1..N different polynomial
		 * functions.
		 * N is the number of given points during the calibration
		 * process.
		 * 
		 * The CSV file consists of N lines, each line contains
		 * the value of the point followed by the coefficients.
		 * 
		 * Example:
		 * With 3 values (points) we have 2 segments (polynomials)
		 * 0.5,4,3,2,1
		 * 0.8,5,4,3,2
		 * 1,,,,
		 * 
		 * Means that:
		 * 
		 * [0.5 <= x < 0.8] f(x) = 4 + 3*x + 2*x^2 + 1*x^3
		 * [0.8 <= x <= 1] f(x) = 5 + 4*x + 3*x^2 + 2*x^3
		 */
		PolynomialSplineFunction splineFunction = (PolynomialSplineFunction) this.function;
		int segments = splineFunction.getN();
		StringBuilder csv = new StringBuilder();
		double[] points = splineFunction.getKnots();
		PolynomialFunction[] polynomials = splineFunction.getPolynomials();
		
		for (int i=0; i<segments; i++) {
			int degree = polynomials[i].degree();
			double coefficients[] = polynomials[i].getCoefficients();
			csv.append(points[i]);
			for (int j=0; j<=degree; j++) {
				csv.append(',');
				csv.append(coefficients[j]);
			}
			csv.append('\n');
		}
		csv.append(points[segments]);
		csv.append(",,,,\n");
		
		return csv.toString();
	}

}
