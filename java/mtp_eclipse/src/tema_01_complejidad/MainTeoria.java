package tema_01_complejidad;

import java.util.Stack;

public class MainTeoria {


	public static int gfsuma_vector_it(int[] v, int currentIndex, int acum)
	{
		int suma = 0;
		if (currentIndex == v.length) {			
			suma = acum;
		}
		else if (currentIndex < v.length) {			
			suma = gfsuma_vector_it(v, currentIndex+1, acum + v[currentIndex]);
		}		
		return suma;		
	}
	
	public static int gfsuma_vector_rec(int[] v)
	{
		int suma = 0;
		int currentIndex = 0;
		while (currentIndex < v.length)
		{
			suma += v[currentIndex];
			currentIndex++;
		}
		return suma;
	}
	
	public static int mcd_rec(int a, int b)
	{
		int mcd = 0;
		
		if (a == b)
			mcd = a;
		else if (a > b)
			mcd = mcd_rec(a - b, b);
		else if (a < b)
			mcd = mcd_rec(a, b - a);
		
		return mcd;
	}
	
	public static int mcd_it(int a, int b)	
	{
		int mcd = 0;
		
		while (a != b)
		{
			if (a > b)
				a -= b;
			else if (a < b)
				b -= a;				
		}
		mcd = a;
		
		return mcd;
	}
	
	public static int suma_digitos_rec(int n)
	{
		int suma = 0;
		if (n < 10)
			suma = n;
		else if (n >= 10)
			suma = suma_digitos_rec(n / 10) + (n % 10);
		return suma;
	}
	
	public static int suma_digitos_it(int n)
	{
		Stack<Integer> stack = new Stack<Integer>();
		int nTemp = n;
		int suma = 0;
		while (nTemp > 10)
		{
			stack.add(nTemp);
			nTemp = nTemp / 10;
		}
		suma = nTemp;
		
		while (!stack.isEmpty())
		{
			nTemp = stack.pop();
			suma = suma + (nTemp % 10);
		}
		
		return suma;
	}
	
	public static void main(String[] args) {
		int[] vectorInt = {1,2,3,4,5,6,7};
		//int resultado = Main.gfsuma_vector_it(vectorInt, 0, 0);
		//int resultado = Main.gfsuma_vector_rec(vectorInt);
		//int resultado = Main.mcd_it(40, 20);
		int resultado = MainTeoria.suma_digitos_it(1234567);
		System.out.println(resultado);
	}

}
