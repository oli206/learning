package tema_08_vueltaatras;

import java.util.Vector;

import common.Arista;
import common.Grafo;
import common.Nodo;

public class MainCicloHamiltonianoVA {

	public static void ciclo_hamiltoniano_va(Grafo g, Vector<Integer> sol, int k, Vector<Boolean> usado) {
		imprimir_sol(sol);
		for (int vertice = 1; vertice <= g.getNodos().size()-1; vertice++) {
			if (!usado.get(vertice)
					&& gv_esta_arista(sol.get(k - 1), vertice, g)) {
				sol.set(k, new Integer(vertice));
				usado.set(vertice, new Boolean(true));

				if (k == g.getNodos().size()-1) {
					if (gv_esta_arista(sol.get(g.getNodos().size()-1), 0, g)) {
						imprimir_sol(sol);
					}
				} else {
					ciclo_hamiltoniano_va(g, sol, k + 1, usado);
				}
				usado.set(vertice, new Boolean(false));
			}
		}
	}

	public static boolean gv_esta_arista(int v1, int v2, Grafo g) {
		for (Arista a : g.getAristas()) {
			if (a.getOrigen().getCodigo() == v1 && a.getDestino().getCodigo() == v2 
					|| a.getOrigen().getCodigo() == v2 && a.getDestino().getCodigo() == v1) {
				return true;
			}
		}

		return false;
	}
	
	public static void imprimir_sol(Vector<Integer> sol)
	{
		for (Integer i : sol)
		{
			System.out.print(i + " ");
		}
		System.out.println("");
	}

	public static void main(String[] args) {
		Nodo n0 = new Nodo(0, "");
		Nodo n1 = new Nodo(1, "");
		Nodo n2 = new Nodo(2, "");
		Nodo n3 = new Nodo(3, "");
		Nodo n4 = new Nodo(4, "");

		Arista a0 = new Arista(0, "", n0, n2);
		Arista a1 = new Arista(1, "", n2, n1);
		Arista a2 = new Arista(2, "", n2, n3);
		Arista a3 = new Arista(3, "", n3, n4);
		Arista a4 = new Arista(4, "", n4, n0);
		Arista a5 = new Arista(5, "", n0, n3);
		Arista a6 = new Arista(6, "", n1, n4);
		Arista a7 = new Arista(7, "", n2, n4);

		Grafo g = new Grafo();
		g.agregarArista(a0);
		g.agregarArista(a1);
		g.agregarArista(a2);
		g.agregarArista(a3);
		g.agregarArista(a4);
		g.agregarArista(a5);
		g.agregarArista(a6);
		g.agregarArista(a7);
		
		g.agregarNodo(n0);
		g.agregarNodo(n1);
		g.agregarNodo(n2);
		g.agregarNodo(n3);
		g.agregarNodo(n4);

		Vector<Integer> sol = new Vector<Integer>();
		for (int i = 0; i <= 4; i++) {
			sol.add(new Integer(0));
		}

		Vector<Boolean> usado = new Vector<Boolean>();
		for (int i = 0; i <= 4; i++) {
			usado.add(new Boolean(false));
		}

		sol.set(0, new Integer(0));
		usado.set(0, new Boolean(true));

		ciclo_hamiltoniano_va(g, sol, 1, usado);
	}
}
