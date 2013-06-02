import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: Oliver
 * Date: 2/06/13
 * Time: 5:15
 * To change this template use File | Settings | File Templates.
 */
public class Zipfsong {

    public static void main (String[] args) throws IOException {
        InputStreamReader inp = new InputStreamReader(System.in);
        BufferedReader br = new BufferedReader(inp);
        String s;
        boolean readNumberOfSongs = false;

        BestSongs bestSongs = null;

        int i = 0;
        while ((s = br.readLine()) != null && s.length() != 0)
        {
            if (!readNumberOfSongs)
            {

                int numberOfSongsInAlbum = Integer.parseInt(s.split(" ")[0]);
                int numberOfSongsToSelect = Integer.parseInt(s.split(" ")[1]);
                bestSongs = new BestSongs(numberOfSongsToSelect);
                readNumberOfSongs = true;
            }
            else
            {
                int expectedTimesListened = 1 / i;
                int numberOfTimesListened = Integer.parseInt(s.split(" ")[0]);
                String nameOfSong = s.split(" ")[1];
                int qualityOfSong = numberOfTimesListened / expectedTimesListened;
                Song song = new Song(qualityOfSong,nameOfSong);

                if (bestSongs.isBetter(song))
                {
                    bestSongs.insertSorted(song);
                }

                i++;
            }
        }

        System.out.println(bestSongs.toString());

    }

    static class Song implements Comparable
    {
        private int quality;
        private String name;

        private Song(int quality, String name) {
            this.quality = quality;
            this.name = name;
        }

        private String getName() {
            return name;
        }

        private int getQuality() {
            return quality;
        }

        @Override
        public int compareTo(Object o) {
            Song s = (Song) o;
            return this.quality - s.quality;  //To change body of implemented methods use File | Settings | File Templates.
        }
    }

    static class BestSongs extends ArrayList {

        public BestSongs(int _size)
        {
            super(_size);
        }

        public boolean isBetter(Song song)
        {
            for (int i = size()-1; i> 0; i --)
            {
                if (song.getQuality() > ((Song) get(i)).getQuality())
                {
                    return true;
                }
            }
            return false;
        }

        @SuppressWarnings("unchecked")
        public void insertSorted(Song value) {
            add(value);
            for (int i = size()-1; i > 0 && value.compareTo(get(i-1)) < 0; i--)
                Collections.swap(this, i, i - 1);
        }

        @Override
        public String toString()
        {
            StringBuilder stringBuilder = new StringBuilder();
            for (int i = size()-1; i> 0; i--)
            {
                stringBuilder.append(((Song) get(i)).getName() + "\n");
            }
            return stringBuilder.toString();
        }
    }
}
