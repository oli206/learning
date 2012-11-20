package common;

import java.io.Serializable;
import java.util.HashMap;


import com.jme.renderer.ColorRGBA;
 
public class Arista implements Serializable {
	private int codigo;
	private String tipo;
	private Nodo origen, destino;
	private ColorRGBA color;
	private HashMap<String, Object> atributos;

	/**
	 * MŽtodo constructor de la clase Arista. Permite crear un objeto arista
	 * @param codigo
	 * 				c—digo de la arista
	 * @param tipo
	 * 				tipo de arista
	 * @param origen
	 * 				nodo origen
	 * @param destino
	 * 				nodo destino
	 */
	public Arista(int codigo, String tipo, Nodo origen, Nodo destino) {
		this.codigo = codigo;
		this.tipo = tipo;
		this.origen = origen;
		this.destino = destino;
		atributos = new HashMap<String, Object>();
	}

	/**
	 * MŽtodo para obtener el tipo de la arista
	 * @return  String con el tipo de arista
	 */
	public String getTipo() {
		return tipo;
	}

	/**
	 * MŽtodo para obtener el codigo de la arista
	 * @return  int con el codigo de la arista  
	 */
	public int getCodigo() {
		return codigo;
	}

	/**
	 * MŽtodo para obtener el nodo origen de la arista
	 * @return  Nodo origen de la arista 
	 */
	public Nodo getOrigen() {
		return origen;
	}

	/**
	 * MŽtodo para obtener el nodo destino de la arista
	 * @return  Nodo destino de la arista  
	 */
	public Nodo getDestino() {
		return destino;
	}

	/**
	 * MŽtodo para obtener los atributos de la arista
	 * @return  HashMap<String, Object> con los atributos de la arista
	 */
	public HashMap<String, Object> getAtributos() {
		return atributos;
	}

	/**
	 * MŽtodo para agregar un atributo a la colecci—n de atributos de la arista
	 * @param nombreAtributo
	 * @param valor
	 */
	public void agregarAtributo(String nombreAtributo, Object valor) {
		atributos.put(nombreAtributo, valor);
	}

	/**
	 * MŽtodos para definir la colecci—n de atributos de la arista
	 * @param atributos
	 */
	public void setAtributos(HashMap<String, Object> atributos) {
		this.atributos = atributos;
	}

	/**
	 * MŽtodos para definir el color de la arista
	 * @param color
	 */
	public void setColor(ColorRGBA color) {
		this.color = color;
	}
	
	/**
	 * MŽtodo para obtener el color de la arista
	 * @return el color definido para el tipo de arista
	 */
	public ColorRGBA getColor(){
		return color;
	}

}