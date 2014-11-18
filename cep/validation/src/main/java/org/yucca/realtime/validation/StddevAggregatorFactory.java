package org.yucca.realtime.validation;

import org.wso2.siddhi.core.query.selector.attribute.factory.OutputAttributeAggregatorFactory;
import org.wso2.siddhi.core.query.selector.attribute.handler.OutputAttributeAggregator;
import org.wso2.siddhi.query.api.definition.Attribute.Type;
import org.wso2.siddhi.query.api.extension.annotation.SiddhiExtension;

@SiddhiExtension(namespace = "yucca", function = "stddev")
public class StddevAggregatorFactory implements
		OutputAttributeAggregatorFactory {

	@Override
	public OutputAttributeAggregator createAttributeAggregator(Type[] arg0) {
		return new StddevAggregatorDouble();
	}

}
