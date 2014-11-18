package org.yucca.realtime.validation;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.math3.stat.descriptive.DescriptiveStatistics;
import org.wso2.siddhi.core.query.selector.attribute.handler.OutputAttributeAggregator;
import org.wso2.siddhi.query.api.definition.Attribute;
import org.wso2.siddhi.query.api.definition.Attribute.Type;

public class StddevAggregatorDouble implements OutputAttributeAggregator {

	private static final long serialVersionUID = 1L;
	private static final Attribute.Type type = Attribute.Type.DOUBLE;
	private List<Double> values = new LinkedList<Double>();
	

	@Override
	public void destroy() {

	}

	@Override
	public Type getReturnType() {
		return type;
	}

	@Override
	public OutputAttributeAggregator newInstance() {
		return new StddevAggregatorDouble();
	}

	@Override
	public synchronized Object processAdd(Object obj) {
		Double value = ((Double) obj);
		values.add(value);
		
		return computeStdDev();
	}

	@Override
	public synchronized Object processRemove(Object obj) {
		values.remove(obj);
		
		return computeStdDev();				
	}
	
	private double computeStdDev() {
		DescriptiveStatistics stats = new DescriptiveStatistics();
		Iterator<Double> i = values.iterator();

		while (i.hasNext()) {
			stats.addValue(i.next().doubleValue());
		}
		
		double stdDev = stats.getStandardDeviation();
		stats = null;
		
		return stdDev;
	}

}
