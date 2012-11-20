package common;

import java.io.Serializable;
import java.util.*;

import com.jme.renderer.ColorRGBA;

public class Nodo implements Serializable {
	private HashMap<String, Object> atributos;
	private String tipo;
	private int codigo;
	private ColorRGBA color;
	private String figura;
	
	/**
	 * MŽtodo para obtener la figura del nodo
	 * @return  String con la figura del nodo  
	 */
	public String getFigura() {
		return figura;
	}

	/**
	 * MŽtodos para definir la figura del nodo
	 * @param figura
	 */
	public void setFigura(String figura) {
		this.figura = figura;
	}

	/**
	 * MŽtodo constructor de la clase Nodo. Permite crear un objeto nodo
	 * @param codigo
	 * 				c—digo del nodo
	 * @param tipo
	 * 				tipo de nodo
	 */
	public Nodo(int codigo, String tipo) {
		this.codigo = codigo;
		this.tipo = tipo;
		atributos = new HashMap<String, Object>();
	}
	
	/**
	 * MŽtodos para definir la colecci—n de atributos del nodo
	 * @param atributos
	 */
	public void setAtributo(HashMap<String, Object> atributos) {
		this.atributos = atributos;
	}
	
	/**
	 * MŽtodo para agregar un atributo a la colecci—n de atributos del nodo
	 * @param nombreAtributo
	 * @param valor
	 */
	public void agregarAtributo(String nombreAtributo, Object valor) {
		atributos.put(nombreAtributo, valor);
	}
	
	/**
	 * MŽtodo para obtener el codigo del nodo
	 * @return  int con el codigo del nodo  
	 */
	public int getCodigo() {
		return codigo;
	}
	
	/**
	 * MŽtodo para obtener el tipo del nodo
	 * @return  String con el tipo del nodo
	 */
	public String getTipo() {
		return tipo;
	}
	
	/**
	 * MŽtodo para obtener los atributos del nodo
	 * @return  HashMap<String, Object> con los atributos del nodo
	 */
	public HashMap<String, Object> getAtributos() {
		return atributos;
	}

	/**
	 * MŽtodos para definir la colecci—n de atributos del nodo
	 * @param atributos
	 */
	public void setAtributos(HashMap<String, Object> atributos) {
		this.atributos = atributos;
		
	}
	
	/**
	 * MŽtodos para definir el color del nodo
	 * @param color
	 */
	public void setColor(ColorRGBA color){
		this.color=color;
	}
	
	/**
	 * MŽtodo para obtener el color del nodo
	 * @return el color definido para el tipo de nodo
	 */
	public ColorRGBA getColor(){
		return color;
	}
	
	
}
