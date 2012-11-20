package tema_01_complejidad;

public class MainEjercicios {

	public static int raiz1(int n, int a)
	{
		int resultado = 0;
		if (Math.pow(a,2) > n)
		{
			resultado = 0;
		}
		if (Math.pow(a,2) <= n)
		{
			int resultadoTemp = raiz1(n,2*a);
			if (n < Math.pow(resultadoTemp + a,2))
			{
				resultado = resultadoTemp;
			}
			else if (n >= (Math.pow(resultadoTemp + a, 2)))
			{
				resultado = resultadoTemp + a;
			}
		}
		
		return resultado;
	}
	
	public static void main(String[] args) {
		int resultado = raiz1(25,1);
		
		System.out.println(resultado);
	}

}
