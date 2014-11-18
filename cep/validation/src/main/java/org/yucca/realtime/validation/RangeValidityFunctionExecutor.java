/**
 * This is a Siddhi extension providing a commodity function to
 * generate validity tag for streams.
 * 
 * Usage:
 *    yucca:range_validity(value, min, max)
 *    
 *    
 * Example ([-inf .. 5]):
 *    
 * from input0
 * select meta_source, time, value, yucca:range_validity(value,-1/0,5) as validity
 * insert into output0
 * 
 */

package org.yucca.realtime.validation;

import org.wso2.siddhi.core.config.SiddhiContext;
import org.wso2.siddhi.core.exception.QueryCreationException;
import org.wso2.siddhi.core.executor.function.FunctionExecutor;
import org.wso2.siddhi.query.api.definition.Attribute;
import org.wso2.siddhi.query.api.definition.Attribute.Type;
import org.wso2.siddhi.query.api.extension.annotation.SiddhiExtension;

@SiddhiExtension(namespace = "yucca", function = "range_validity")
public class RangeValidityFunctionExecutor extends FunctionExecutor {

	private static final Attribute.Type type = Attribute.Type.STRING;
	
	@Override
	public Type getReturnType() {
		return type;
	}

	@Override
	public void destroy() {

	}

	@Override
	public void init(Type[] types, SiddhiContext ctx) {
		for (Attribute.Type attributeType : types) {
			if (attributeType == Attribute.Type.BOOL || attributeType == Attribute.Type.STRING) {
				throw new QueryCreationException("range_validity cannot have parameters with types String or Bool");
			}
		}

	}

	@Override
	protected Object process(Object obj) {
		Object[] params = (Object[]) obj;
		
		Double val = Double.parseDouble(String.valueOf(params[0]));
		Double min = Double.parseDouble(String.valueOf(params[1]));
		Double max = Double.parseDouble(String.valueOf(params[2]));
		
		String validity = "valid";
		if (val < min || val > max) {
			validity = "erroneous";
		}
		
		return validity;
	}

}
