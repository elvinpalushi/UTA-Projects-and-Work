package mdi;

import library.Library;
import library.Patron;
import library.Publication;
import library.Video;
import library.InvalidRuntimeException;

import java.util.Scanner;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class LibraryManager 
{
    private static Scanner scanner = new Scanner(System.in);
    private static final String name = "The Bodleian Library";
    private Library library;

    public LibraryManager(Library library)
    {
        this.library = library;
    }

    public void listPublications()
    {
        System.out.println("\n" + library);
    }

    public void addPublication()
    {
        scanner.nextLine();

        System.out.print("Enter publication title: ");
        String title = scanner.nextLine();
            
        System.out.print("Enter author: ");
        String author = scanner.nextLine();
            
        System.out.print("Enter copyright year: ");
        int copyright = scanner.nextInt();
        scanner.nextLine();
            
        Publication publication = new Publication(title, author, copyright);
        library.addPublication(publication);
    }

    public void addVideo()
    {
        scanner.nextLine();

        System.out.print("Enter the video title: ");
        String title = scanner.nextLine();

        System.out.print("Enter the author: ");
        String author = scanner.nextLine();

        System.out.print("Enter the copyright year: ");
        int copyright = scanner.nextInt();
        scanner.nextLine();

        System.out.print("Enter the runtime: ");
        int runtime = scanner.nextInt();
        scanner.nextLine();

        Video video = new Video(title, author, copyright, runtime);
        library.addPublication(video);
    }

    public void checkOutPublication()
    {
        System.out.print("Which book would you like to check out? ");
        
        int publicationIndex = scanner.nextInt();
        scanner.nextLine();
            
        System.out.println(library.patronMenu());
        
        System.out.print("Who are you? ");
            
        int patronIndex = scanner.nextInt();
        scanner.nextLine();
        
        library.checkOut(publicationIndex, patronIndex);
    }

    public void checkInPublication()
    {
        System.out.print("Which book would you like to check in? ");
        
        int publicationIndex = scanner.nextInt();
        scanner.nextLine();
        
        library.checkIn(publicationIndex);
    }

    public void listPatrons()
    {
        System.out.println(library.patronMenu());
    }

    public void addPatron()
    {
        scanner.nextLine();

        System.out.print("Enter patron name: ");
        String name = scanner.nextLine();

        System.out.print("Enter patron email: ");
        String email = scanner.nextLine();

        Patron patron = new Patron(name, email);
        library.addPatron(patron);
    }

    public void openLibrary()
    {
        scanner.nextLine();
        System.out.print("Open filename: ");
        String filename = scanner.nextLine();

        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            Library newLibrary = new Library(br);
            library = newLibrary;
        } catch (IOException e) {
            System.err.println("File " + filename + " can't be read!\n");
        }
    }

    public void saveLibrary()
    {
        scanner.nextLine();
        System.out.print("Save filename: ");
        String filename = scanner.nextLine();

        try (BufferedWriter bw = new BufferedWriter(new FileWriter(filename))) {
            library.save(bw);
        } catch (IOException  e) {
            System.err.println("File " + filename + " can't be written!\n");
        }
    }

    public static void main(String[] args) 
    {
        LibraryManager menu = new LibraryManager(new Library(name));
        while(true) {
            try {
                System.out.println("\n\n\n\n\n\n\n\nM A I N   M E N U\n\n" + name + "\n");

                System.out.println("Publications");
                System.out.println("1) List");
                System.out.println("2) Add Publication");
                System.out.println("3) Add Video");
                System.out.println("4) Check Out");
                System.out.println("5) Check In\n");

                System.out.println("Patrons");
                System.out.println("6) List");
                System.out.println("7) Add\n");

                System.out.println("Files");
                System.out.println("8) Open Library");
                System.out.println("9) Save Library");
                System.out.println("0) Exit\n\n");

                System.out.print("Selection: ");

                int selection = scanner.nextInt();

                switch (selection)
                {
                    case 1:
                        menu.listPublications();
                        break;
                    case 2:
                        menu.addPublication();
                        break;
                    case 3:
                        menu.addVideo();
                        break;
                    case 4:
                        menu.checkOutPublication();
                        break;
                    case 5:
                        menu.checkInPublication();
                        break;
                    case 6:
                        menu.listPatrons();
                        break;
                    case 7:
                        menu.addPatron();
                        break;
                    case 8:
                        menu.openLibrary();
                        break;
                    case 9:
                        menu.saveLibrary();
                        break;
                    case 0:
                        System.exit(0);
                        break;
                    default:
                        System.out.println("Invalid selection, try again!");
                        break;
                }
            } catch (Exception e) {
                System.err.println("Invalid selection: " + e);
            } 
        }
    }
}