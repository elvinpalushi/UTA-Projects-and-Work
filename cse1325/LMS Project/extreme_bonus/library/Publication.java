package library;

import java.time.LocalDate;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;

/**
 * A library resource that can be checked out by a patron.
 *
 * @author             Elvin Palushi
 * @version            1.0
 * @since              1.0
 * @license.agreement  Gnu General Public License 3.0
 */
public class Publication 
{
    private String title;
    private String author;
    private int copyright;
    private Patron loanedTo;
    private LocalDate dueDate;
    
    /**
     * Creates a Publication instance.
     *
     * @param title            the title of a publication
     * @param author           the author of a publication
     * @param copyright        the copyright year of a publication
     * @since          1.0
     */
    public Publication(String title, String author, int copyright) 
    {
        this.title = title;
        this.author = author;
        this.copyright = copyright;

        if (copyright < 1900 || copyright > LocalDate.now().getYear()) throw new IllegalArgumentException("Year is not within the range of 1900 to the present year");
    }

    /**
     * Creates a Publication instance by reading data from a BufferedReader stream.
     *
     * @param br             a BufferedReader used to read patron data from a file
     * @throws IOException   throws an IOException if an I/O error occurs while reading data from the stream
     * @since                1.0
     */
    public Publication(BufferedReader br) throws IOException
    {
        this.title = br.readLine();
        this.author = br.readLine();
        this.copyright = Integer.parseInt(br.readLine());

        if("checked in".equals(br.readLine()))
        {
            this.loanedTo = null;
            this.dueDate = null;
        }
        else
        {
            this.loanedTo = new Patron(br);
            this.dueDate = LocalDate.parse(br.readLine());
        }
    }

    /**
     * Saves publication data to a BufferedWriter stream.
     *
     * @param bw             a BufferedWriter used to write patron data to a file
     * @throws IOException   throws an IOException if an I/O error occurs while writing data from the stream
     * @since                1.0
     */
    public void save(BufferedWriter bw) throws IOException
    {
        bw.write("" + title + '\n');
        bw.write("" + author + '\n');
        bw.write("" + copyright + '\n');

        if (loanedTo == null)
        {
            bw.write("checked in\n");
        }
        else
        {
            bw.write("checked out\n");
            loanedTo.save(bw);
            bw.write("" + dueDate.toString() + '\n');
        }
    }

     /**
     * Checks out a publication by a patron and sets the due date 14 days from now.
     *
     * @param patron       the publication that will be checked out from the library
     * @since              1.0
     */ 
    public void checkOut(Patron patron) 
    {
        loanedTo = patron;
        dueDate = LocalDate.now().plusDays(14);
    }

    /**
     * Checks in a publication and sets the due date to null.
     *
     * @since              1.0
     */ 
    public void checkIn() 
    {
        loanedTo = null;
        dueDate = null;
    }

    /**
     * Builds toString() that is used to print the publications in the library.
     *
     * @param pre       the publication's title, author, and copyright year
     * @param mid       the patron the publication is loaned to as well as the due date
     * @return         String the title, author, and copyright year of a publication, and if not checked out, the patron the publication is loaned to,
     *  along with the due date
     * @since          1.0
     */
    protected String toStringBuilder(String pre, String mid)
    {
        StringBuilder sb = new StringBuilder();

        sb.append(pre);
        sb.append(String.format("\"%s\" by %s, copyright %d", title, author, copyright));
        sb.append(mid);
        
        if (loanedTo != null)
        {
            sb.append(String.format("\n     : loaned to %s until %s", loanedTo, dueDate));
        }
        
        return sb.toString();
    }

    /**
     * Returns a Book publication.
     *
     * @return         String a Book publication
     * @since          1.0
     */
    @Override
    public String toString()
    {
        return toStringBuilder("Book ", "");
    }
}