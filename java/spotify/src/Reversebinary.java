import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created with IntelliJ IDEA.
 * User: Oliver
 * Date: 2/06/13
 * Time: 4:50
 * To change this template use File | Settings | File Templates.
 */

public class Reversebinary {

    public static void main (String[] args) throws IOException {
        InputStreamReader inp = new InputStreamReader(System.in);
        BufferedReader br = new BufferedReader(inp);
        String str = br.readLine();

        int input = Integer.parseInt(str);
        String binaryInput = Integer.toBinaryString(input);
        String binaryOutput = new StringBuilder(binaryInput).reverse().toString();
        int output = Integer.parseInt(binaryOutput,2);
        System.out.println(output);
    }
}
