package library;

import java.time.Duration;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;

/**
 * A library video that can be checked out by a patron.
 *
 * @author             Elvin Palushi
 * @version            1.0
 * @since              1.0
 * @license.agreement  Gnu General Public License 3.0
 */
public class Video extends Publication 
{
    private Duration runtime;

    /**
     * Creates a Video instance.
     *
     * @param title            the title of a publication
     * @param author           the author of a publication
     * @param copyright        the copyright year of a publication
     * @param runtime          the runtime, in minutes, of a publication
     * @since              1.0
     */
    public Video(String title, String author, int copyright, int runtime) 
    {
        super(title, author, copyright);

        if (runtime <= 0)
        {
            throw new InvalidRuntimeException(title, runtime);
        }

        this.runtime = Duration.ofMinutes(runtime);
    }

    /**
     * Creates a Video instance by reading data from a BufferedReader stream.
     *
     * @param br             a BufferedReader used to read patron data from a file
     * @throws IOException   throws an IOException if an I/O error occurs while reading data from the stream
     * @since                1.0
     */
    public Video(BufferedReader br) throws IOException
    {
        super(br);
        this.runtime = Duration.ofMinutes(Long.parseLong(br.readLine()));
    }

    /**
     * Saves Video data to a BufferedWriter stream.
     *
     * @param bw             a BufferedWriter used to write patron data to a file
     * @throws IOException   throws an IOException if an I/O error occurs while writing data from the stream
     * @since                1.0
     */
    public void save(BufferedWriter bw) throws IOException
    {
        super.save(bw);
        bw.write("" + runtime.toMinutes() + '\n');
    }

    /**
     * Returns a Video publication and the runtime of the video.
     *
     * @return         String a Video publication and runtime
     * @since          1.0
     */
    @Override
    public String toString()
    {
        String mid = String.format(", runtime %d minutes", runtime.toMinutes());
        return toStringBuilder("Video ", mid);
    }
}