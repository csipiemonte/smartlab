/**
 * Validator.java
 * 
 * This class implements different validation checks on numeric
 * time series.
 * 
 */
package it.smartdatanet.smartlab.validation;

import org.apache.commons.math3.stat.descriptive.DescriptiveStatistics;

public class Validator {
	
	public static final String UNKNOWN = "unknown";
	public static final String VALID = "valid";
	public static final String ERRONEOUS = "erroneous";
	public static final String DOUBTFUL = "doubtful";
	
	private double min;
	private double max;
	private double dTStats;  // Time window (seconds) for stats
	private double dTSpeed;  // Time window (seconds) for speed
	private double fps;      // Frames per second
	private int n;           // |value-mean| < n*sigam
	private double minSpeed;
	private double maxSpeed;
	
	private DescriptiveStatistics stats;
	private DescriptiveStatistics speedStats;
	private double lastValue;
	private int win;
	private int speedWin;
	
	/**
	 * The constructor will create a set of defaults parameters used
	 * for validity checks
	 */
	public Validator() {
		min = Double.NEGATIVE_INFINITY;
		max = Double.POSITIVE_INFINITY;
		fps = 0;
		dTStats = 0;
		dTSpeed = 0;
		n = 0;
		lastValue = Double.NaN;
		minSpeed = Double.NEGATIVE_INFINITY;
		maxSpeed = Double.POSITIVE_INFINITY;
	}
	
	/**
	 * The allowed min limit
	 */
	public void setMin(double min) {
		this.min = min;
	}
	
	/**
	 * The allowed max limit
	 */
	public void setMax(double max) {
		this.max = max;
	}
	
	/**
	 * Samples per second
	 */
	public void setFps(double fps) {
		this.fps = fps;
	}
	
	/**
	 * Time interval in seconds to observe descriptive
	 * statistics (mean,standard deviation ...)
	 */
	public void setdTStats(double dTStats) {
		this.dTStats = dTStats;
	}
	
	/**
	 * Time interval in seconds to observe descriptive
	 * statistics of the speed (derivative)
	 */
	public void setdTSpeed(double dTSpeed) {
		this.dTSpeed = dTSpeed;
	}
	
	/**
	 * This is the N multiplier for the following check:
	 * 
	 * | Value - Mean | < N * StandardDeviation
	 */
	public void setN(int n) {
		this.n = n;
	}
	
	/**
	 * Min variation of the speed in the observation interval
	 */
	public void setMinSpeed(double minSpeed) {
		this.minSpeed = minSpeed;
	}
	
	/**
	 * Max variation of the speed in the observation interval
	 */
	public void setMaxSpeed(double maxSpeed) {
		this.maxSpeed = maxSpeed;
	}
	
	/**
	 * Based on observation time intervals and fps this method will create
	 * two statistics contexts with the correct sliding window
	 */
	public void init() {
		
		win = (int) (fps * dTStats);
		stats = new DescriptiveStatistics(win);
		speedWin = (int) (fps * dTSpeed);
		speedStats = new DescriptiveStatistics(speedWin);
	}

	/**
	 * Returns the validity tag for the given value
	 */
	public String getValidity(double value) {
		
		// out-of-range data do not update statistics
		if (isOutOfRange(value)) {
			return ERRONEOUS;
		}
		
		//TODO: Missing samples or errors creates discontinuity in all stats
		stats.addValue(value);
		if (win > 0) {
			win--;
		}
		
		// Compute stats for the speed
		if (! Double.isNaN(lastValue)) {
			double speed = (value - lastValue) * fps;
			if (speed < 0) {
				speed *= -1;
			}
			speedStats.addValue(speed);
		} else {
			speedStats.addValue(0);
		}
		lastValue = value;
		if (speedWin > 0) {
			speedWin--;
		}

		
		if (isOutNSigma(value) || isSpeedOutOfRange()) {
			return DOUBTFUL;
		}
		
		return VALID;
	}
	
	/**
	 * True if
	 * |Value - Mean| > N * StandardDeviation
	 */
	private boolean isOutNSigma(double value) {
		// We need to reach the observation window before
		if (n == 0 || win > 0) {
			return false;
		}
		
		double mean = stats.getMean();
		double sigma = stats.getStandardDeviation();
				
		// | value - mean |
		double delta = value - mean;
		if (delta < 0) {
			delta *= -1;
		}
		
		return (delta > (n * sigma));
	}
	
	/**
	 * True if the value is out of range
	 */
	private boolean isOutOfRange(double value) {
		return (value < min) || (value > max);
	}
	
	/**
	 * True if the speed variation is out of range
	 * inside the observation interval
	 */
	private boolean isSpeedOutOfRange() {
		// We need to reach the speed observation time
		if (speedWin > 0) {
			return false;
		}
		double minSpeed = speedStats.getMin();
		double maxSpeed = speedStats.getMax();
		return (minSpeed < this.minSpeed ) || (maxSpeed > this.maxSpeed);
	}
	
	@Override
	public String toString() {
		StringBuilder ret = new StringBuilder();
		ret.append("\nMin: ");
		ret.append(min);
		ret.append("\nMax: ");
		ret.append(max);
		ret.append("\nFPS: ");
		ret.append(fps);
		ret.append("\nN-Sigma: ");
		ret.append(n);
		ret.append("\nDeltaT (stats): ");
		ret.append(dTStats);
		ret.append("\nDeltaT (speed): ");
		ret.append(dTSpeed);
		ret.append("\nMin speed: ");
		ret.append(minSpeed);
		ret.append("\nMax speed: ");
		ret.append(maxSpeed);
		ret.append("\n");
		
		return ret.toString();
	}

}
