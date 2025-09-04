package library;

/**
 * A library resource that can be checked out by a patron.
 *
 * @author             Elvin Palushi
 * @version            1.0
 * @since              1.0
 * @license.agreement  Gnu General Public License 3.0
 */
public class InvalidRuntimeException extends ArithmeticException
{
    /**
     * Creates a new InvalidRuntimeException with no specified message.
     *
     * @since              1.0
     */ 
    public InvalidRuntimeException()
    {
        super();
    }

    /**
     * Creates a new InvalidRuntimeException with the specified message.
     *
     * @param message      the specified message
     * @since              1.0
     */ 
    public InvalidRuntimeException(String message)
    {
        super(message);
    }

    /**
     * Creates a new InvalidRuntimeException that says a video has an invalid runtime.
     *
     * @param title        the title of the publication
     * @param runtime      the runtime
     * @since              1.0
     */ 
    public InvalidRuntimeException(String title, int runtime)
    {
        super(String.format("%s has invalid runtime %d", title, runtime));
    }
}