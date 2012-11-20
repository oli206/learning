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
	 * M�todo para obtener la figura del nodo
	 * @return  String con la figura del nodo  
	 */
	public String getFigura() {
		return figura;
	}

	/**
	 * M�todos para definir la figura del nodo
	 * @param figura
	 */
	public void setFigura(String figura) {
		this.figura = figura;
	}

	/**
	 * M�todo constructor de la clase Nodo. Permite crear un objeto nodo
	 * @param codigo
	 * 				c�digo del nodo
	 * @param tipo
	 * 				tipo de nodo
	 */
	public Nodo(int codigo, String tipo) {
		this.codigo = codigo;
		this.tipo = tipo;
		atributos = new HashMap<String, Object>();
	}
	
	/**
	 * M�todos para definir la colecci�n de atributos del nodo
	 * @param atributos
	 */
	public void setAtributo(HashMap<String, Object> atributos) {
		this.atributos = atributos;
	}
	
	/**
	 * M�todo para agregar un atributo a la colecci�n de atributos del nodo
	 * @param nombreAtributo
	 * @param valor
	 */
	public void agregarAtributo(String nombreAtributo, Object valor) {
		atributos.put(nombreAtributo, valor);
	}
	
	/**
	 * M�todo para obtener el codigo del nodo
	 * @return  int con el codigo del nodo  
	 */
	public int getCodigo() {
		return codigo;
	}
	
	/**
	 * M�todo para obtener el tipo del nodo
	 * @return  String con el tipo del nodo
	 */
	public String getTipo() {
		return tipo;
	}
	
	/**
	 * M�todo para obtener los atributos del nodo
	 * @return  HashMap<String, Object> con los atributos del nodo
	 */
	public HashMap<String, Object> getAtributos() {
		return atributos;
	}

	/**
	 * M�todos para definir la colecci�n de atributos del nodo
	 * @param atributos
	 */
	public void setAtributos(HashMap<String, Object> atributos) {
		this.atributos = atributos;
		
	}
	
	/**
	 * M�todos para definir el color del nodo
	 * @param color
	 */
	public void setColor(ColorRGBA color){
		this.color=color;
	}
	
	/**
	 * M�todo para obtener el color del nodo
	 * @return el color definido para el tipo de nodo
	 */
	public ColorRGBA getColor(){
		return color;
	}
	
	
}
