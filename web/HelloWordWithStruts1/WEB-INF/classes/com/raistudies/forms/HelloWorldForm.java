package com.raistudies.forms;

import org.apache.struts.action.ActionForm;

public class HelloWorldForm extends ActionForm {

	private static final long serialVersionUID = 7352021000623040587L;
	
	private String hello = null;

	public HelloWorldForm() {
		super();
		hello = "Hello World";
	}
	
	public String getHello() {
		return hello;
	}

	public void setHello(String hello) {
		this.hello = hello;
	}

}
