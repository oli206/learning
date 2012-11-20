package common;

import java.io.Serializable;
import java.util.HashMap;
import java.util.HashSet;

import com.jme.renderer.ColorRGBA;
 
public class Grafo implements Serializable{

        private HashSet<Nodo> nodos;
        private HashSet<Arista> aristas;
        private HashMap<String,ColorRGBA> coloresNodos;
        private HashMap<String,ColorRGBA> coloresAristas;
        
        /**
         * M�todo para obtener el color de los nodos
         * @return  HashMap<String, ColorRGBA> con el tipo del nodo como clave y el color como valor
         */
        public HashMap<String, ColorRGBA> getColoresNodos() {
                return coloresNodos;
        }

        /**
         * M�todo para asignar la colecci�n con los colores de los nodos a la propiedad del grafo
         * @param coloresNodos
         *                                      Colecci�n con los colores de los nodos
         */
        public void setColoresNodos(HashMap<String, ColorRGBA> coloresNodos) {
                this.coloresNodos = coloresNodos;
        }

        /**
         * * M�todo para obtener el color de las aristas
         * @return  HashMap<String, ColorRGBA> con el tipo de arista como clave y el color como valor 
         */
        public HashMap<String, ColorRGBA> getColoresAristas() {
                return coloresAristas;
        }

        /**
        * M�todo para asignar la colecci�n con los colores de las aristas a la propiedad del grafo
         * @param coloresAristas
         *                                      Colecci�n con los colores de las aristas 
         */
        public void setColoresAristas(HashMap<String, ColorRGBA> coloresAristas) {
                this.coloresAristas = coloresAristas;
        }

        /**
         * Constructor de la clase Grafo para crear un grafo con nodos y aristas 
         */
        public Grafo() {
                nodos = new HashSet<Nodo>();
                aristas = new HashSet<Arista>();
        }

        /**
         * M�todo que permite agregar una arista a la colecci�n de aristas
         * @param arista
         *                              Arista a agregar
         * @return  true si se agreg� la arista, false si hubo alg�n error
         */
        public boolean agregarArista(Arista arista) {
                return aristas.add(arista);
        }

        /**
         * M�todo que permite agregar un nodo a la colecci�n de nodos
         * @param nodo
         *                              Nodo a agregar
         * @return  true si se agreg� el nodo, false si hubo alg�n error
         */
        public boolean agregarNodo(Nodo nodo) {
                return nodos.add(nodo);
        }

        /**
         * M�todo para obtener la colecci�n de nodos del grafo
         * @return HashSet<Nodo> colecci�n con los nodos del grafo 
         */
        public HashSet<Nodo> getNodos() {
                return nodos;
        }

        /**
         * M�todo para obtener la colecci�n de aristas del grafo
         * @return HashSet<Arista> colecci�n con las aristas del grafo  
         */
        public HashSet<Arista> getAristas() {
                return aristas;
        }

        /**
         * M�todo para retornar un Nodo dado un c�digo
         * @param codigo
         *                              C�digo del nodo
         * @return  El nodo correspondiente al c�digo por par�metro
         */
        public Nodo getNodo(int codigo) {
                for (Nodo nodo : nodos) {
                        if (nodo.getCodigo() == codigo)
                                return nodo;
                }
                return null;
        }

        /**
         * M�todo para asignar la colecci�n de nodos 
         * @param  nodos
         */
        public void setNodos(HashSet<Nodo> nodos) {
                this.nodos = nodos;

        }

        /**
         * M�todo para asignar la colecci�n de aristas 
         * @param  aristas
         */
        public void setAristas(HashSet<Arista> aristas) {
                this.aristas = aristas;

        }

        /**
         * M�todo para obtener una arista dado un c�digo
         *  @param codigo
         *                              C�digo de la arista
         * @return  La arista correspondiente al c�digo por par�metro
         */
        public Arista getArista(int codigo) {
                for (Arista arista : aristas) {
                        if (arista.getCodigo() == codigo)
                                return arista;
                }
                return null;
        }

}
