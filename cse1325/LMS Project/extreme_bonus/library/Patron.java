package library;

import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;

/**
 * A person authorized to check out a library resource.
 *
 * @author             Elvin Palushi
 * @version            1.0
 * @since              1.0
 * @license.agreement  Gnu General Public License 3.0
 */
public class Patron
{
    private String name;
    private String email;

    /**
     * Creates a Patron instance.
     *
     * @param name            the name of an authorized patron
     * @param email           the email of an authorized patron
     * @since              1.0
     */ 
    public Patron(String name, String email)
    {
        this.name = name;
        this.email = email;
    }

    /**
     * Creates a Patron instance by reading data from a BufferedReader stream.
     *
     * @param br             a BufferedReader used to read patron data from a file
     * @throws IOException   throws an IOException if an I/O error occurs while reading data from the stream
     * @since                1.0
     */
    public Patron(BufferedReader br) throws IOException
    {
        this.name = br.readLine();
        this.email = br.readLine();
    }

    /**
     * Saves a patron name and email to a BufferedWriter stream.
     *
     * @param bw             a BufferedWriter used to write patron data to a file
     * @throws IOException   throws an IOException if an I/O error occurs while writing data from the stream
     * @since                1.0
     */
    public void save(BufferedWriter bw) throws IOException
    {
        bw.write("" + name + '\n');
        bw.write("" + email + '\n');
    }

    /**
     * Returns the name and email of a patron who s authorized to check out a library resource.
     *
     * @return         String the name and email of a patron
     * @since          1.0
     */
    @Override
    public String toString()
    {
        return String.format("%s (%s)", name, email);
    }
}