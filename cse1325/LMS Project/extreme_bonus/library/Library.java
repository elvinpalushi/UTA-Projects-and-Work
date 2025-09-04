package library;

import java.util.ArrayList;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;

/**
 * Models a library containing various publications.
 *
 * @author             Elvin Palushi
 * @version            1.0
 * @since              1.0
 * @license.agreement  Gnu General Public License 3.0
 */
public class Library
{
    private String name;
    private ArrayList<Publication> publications;
    private ArrayList<Patron> patrons;


    /**
     * Creates a Library instance.
     *
     * @param name            the name of the library
     * @since              1.0
     */
    public Library(String name)
    {
        this.name = name;
        this.publications = new ArrayList<>();
        this.patrons = new ArrayList<>();
    }

    /**
     * Creates a Library instance by reading data from a BufferedReader stream.
     *
     * @param br             a BufferedReader used to read patron data from a file
     * @throws IOException   throws an IOException if an I/O error occurs while reading data from the stream
     * @since                1.0
     */
    public Library(BufferedReader br) throws IOException
    {
        this(br.readLine());

        int size = Integer.parseInt(br.readLine());

        publications.clear();

        for (int i = 0; i < size; i++)
        {
            if (br.readLine().equals("video"))
            {
                publications.add(new Video(br));
            }
            else
            {
                publications.add(new Publication(br));
            }
        }

        patrons.clear();

        int patronSize = Integer.parseInt(br.readLine());

        for (int i = 0; i < patronSize; i++)
        {
            patrons.add(new Patron(br));
        }
    }

    /**
     * Saves Library data to a BufferedWriter stream.
     *
     * @param bw             a BufferedWriter used to write patron data to a file
     * @throws IOException   throws an IOException if an I/O error occurs while writing data from the stream
     * @since                1.0
     */
    public void save(BufferedWriter bw) throws IOException
    {
        bw.write("" + name + '\n');
        bw.write("" + publications.size() + "\n");

        for (Publication publication : publications)
        {
            if (publication instanceof Video)
            {
                bw.write("video\n");
            }
            else
            {
                bw.write("publication\n");
            }

            publication.save(bw);
        }

        bw.write("" + patrons.size() + "\n");

        for (Patron patron : patrons)
        {
            patron.save(bw);
        }
    }

    /**
     * Adds a publication to all publications in the library.
     *
     * @param publication            the publication that will be added to the library
     * @since              1.0
     */ 
    public void addPublication(Publication publication)
    {
        publications.add(publication);
    }

    /**
     * Adds a Patron to all patrons in the library.
     *
     * @param patron            the patron that will be added to the library
     * @since              1.0
     */ 
    public void addPatron(Patron patron)
    {
        patrons.add(patron);
    }

    /**
     * Prints a patron menu of all patrons in the library.
     *
     * @return         String all patrons in the library
     * @since              1.0
     */ 
    public String patronMenu()
    {
        String patronInfo = "Patrons" + "\n\n";

        for (int i = 0; i < patrons.size(); i++)
        {
            Patron patron = patrons.get(i);
            patronInfo += i + ") " + patron + "\n";
        }

        return patronInfo;
    }

    /**
     * Checks out a publication by a patron.
     *
     * @param publicationIndex       the publication that will be checked out from the library
     * @param patronIndex            the patron who will check out the publication from the library
     * @since              1.0
     */ 
    public void checkOut(int publicationIndex, int patronIndex)
    {
        Publication publication = publications.get(publicationIndex);
        Patron patron = patrons.get(patronIndex);
        publication.checkOut(patron);
    }

    /**
     * Checks in a publication by a patron.
     *
     * @param publicationIndex       the publication that will be checked in from the library
     * @since              1.0
     */ 
    public void checkIn(int publicationIndex)
    {
        Publication publication = publications.get(publicationIndex);
        publication.checkIn();
    }

    /**
     * Prints a menu of all publications in the library.
     *
     * @return         String the name of the library and the publications
     * @since          1.0
     */
    @Override
    public String toString()
    {
        String libraryInfo = name + "\n";

        for (int i = 0; i < publications.size(); i++)
        {
            Publication publication = publications.get(i);
            libraryInfo += i + ") " + publication + "\n";
        }

        return libraryInfo;
    }
}