package printtokens;

import org.junit.jupiter.api.*;
import org.junit.jupiter.api.io.TempDir;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

public class PrinttokensE2ETest {

    private final Printtokens pt = new Printtokens();
    private final ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    private final ByteArrayOutputStream errContent = new ByteArrayOutputStream();
    private final PrintStream originalOut = System.out;
    private final PrintStream originalErr = System.err;
    private final InputStream originalIn = System.in;

    @BeforeEach
    public void setUpStreams() {
        outContent.reset();
        errContent.reset();

        System.setOut(new PrintStream(outContent));
        System.setErr(new PrintStream(errContent));
    }

    @AfterEach
    public void restoreStreams() {
        System.setOut(originalOut);
        System.setErr(originalErr);
        System.setIn(originalIn);
    }

    private Path createTempFile(Path tempDir, String prefix, String content) throws IOException {
        Path tempFile = tempDir.resolve(prefix + ".txt");
        Files.writeString(tempFile, content);
        return tempFile;
    }

    private void runMainAndAssert(String[] args, String expectedOutput) {
         try {
             Printtokens.main(args);
         } catch (NullPointerException e) {
             System.err.println("Caught expected NPE during main execution: " + e.getMessage());
         }

        String actual = outContent.toString().replace("\r\n", "\n").trim();
        String expected = expectedOutput.replace("\r\n", "\n").trim();
        assertEquals(expected, actual);
    }

    @Test
    public void testInvalidFileName() {
        String nonExistentFileName = "non_existent_file_12345.txt";
        String[] args = {nonExistentFileName};

        try {
            Printtokens.main(args);
        } catch (NullPointerException e) {
            System.err.println("Caught expected NPE in testInvalidFileName: " + e.getMessage());
        }

        String stdOutput = outContent.toString();
        String errorOutput = errContent.toString();

        assertTrue(stdOutput.contains("The file " + nonExistentFileName +" doesn't exists"),
                   "Expected custom error message in System.out");

        assertTrue(errorOutput.contains("java.io.FileNotFoundException"),
                   "Expected FileNotFoundException stack trace in System.err");
    }

    @Test
    public void testStdInputKeyword() {
        String input = "and\n";
        InputStream in = new ByteArrayInputStream(input.getBytes());
        System.setIn(in);

        String[] args = {};
        String expectedOutput = "keyword,\"and\".";

        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputSpecialSymbols(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test3", "[]");
        String[] args = {inputFile.toString()};
        String expectedOutput = "lsquare.\nrsquare.";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputIdentifier(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test4", "a2 ");
        String[] args = {inputFile.toString()};
        String expectedOutput = "identifier,\"a2\".";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputNumericThenSpecial(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test5", "123(");
        String[] args = {inputFile.toString()};
        String expectedOutput = "numeric,123.\nlparen.";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputStringThenSpecial(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test6", "\"hello\"]");
        String[] args = {inputFile.toString()};
        String expectedOutput = "string,\"hello\".\nrsquare.";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputCharThenSpecial(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test7", "#a'");
        String[] args = {inputFile.toString()};
        String expectedOutput = "character,\"a\".\nquote.";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputCommentEOF(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test8", ";");
        String[] args = {inputFile.toString()};
        String expectedOutput = "comment,\";\".";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputUnterminatedStringEOF(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test9", "\"abc\"");
        String[] args = {inputFile.toString()};
        String expectedOutput = "string,\"abc\".";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputCommentNewline(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test10", ";this is a comment\n");
        String[] args = {inputFile.toString()};
        String expectedOutput = "comment,\";this is a comment\".";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputCommaEOF(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test11", ",");
        String[] args = {inputFile.toString()};
        String expectedOutput = "comma.";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputError(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test12", "112A");
        String[] args = {inputFile.toString()};
        String expectedOutput = "error,\"112A\".";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testFileInputFull(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test13", "and`and\nj\n112A\n)\n,\nor\nif\nxor\nlambda\n=>\n#c\na!\na2\naa\n");
        String[] args = {inputFile.toString()};
        String expectedOutput =
            "keyword,\"and\".\n" +
            "bquote.\n" +
            "keyword,\"and\".\n" +
            "identifier,\"j\".\n" +
            "error,\"112A\".\n" +
            "rparen.\n" +
            "comma.\n" +
            "keyword,\"or\".\n" +
            "keyword,\"if\".\n" +
            "keyword,\"xor\".\n" +
            "keyword,\"lambda\".\n" +
            "keyword,\"=>\".\n" +
            "character,\"c\".\n" + 
            "error,\"a!\".\n" +
            "identifier,\"a2\".\n" +
            "identifier,\"aa\".\n";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testTokenEndBranches(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test_token_end", 
            "\"string\"\n" +        
            "\"unterminated\n" +    
            "; comment\n" +         
            "#c\n" +               
            " \n" +                 
            ";\n" +                
            "plain\n"          
        );
    
        String[] args = {inputFile.toString()};
        String expectedOutput =
            "string,\"string\".\n" +
            "error,\"\"unterminated\".\n" +
            "comment,\"; comment\".\n" +
            "character,\"c\".\n" +
            "comment,\";\".\n" +
            "identifier,\"plain\".\n";
    
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testIsTokenEndAllBranches(@TempDir Path tempDir) throws IOException {
        Path inputFile = tempDir.resolve("test_token_end.txt");
        try (FileWriter writer = new FileWriter(inputFile.toFile())) {
            writer.write(
                "\"hello\"\n" +          
                "\"abc\t\"\n" +          
                "\"abc\n\"\n" +          
                "\"abc\r\"\n" +         
                "\"abcx\n" +            
                ";comment\n" +          
                ";comment\r" +         
                ";comment\t" +          
                ";abc\n" +            
                "(\n" +                
                " \n" +                 
                ";\n" +               
                "plain\n"                
            );
        }
    
        String[] args = {inputFile.toString()};
        String expectedOutput =
            "string,\"hello\".\n" +
            "error,\"\"abc\".\n" +
            "error,\"\"\".\n" +
            "error,\"\"abc\".\n" +
            "error,\"\"\".\n" +
            "error,\"\"abc\".\n" +
            "error,\"\"\".\n" +
            "error,\"\"abcx\".\n" +
            "comment,\";comment\".\n" +
            "comment,\";comment\".\n" +
            "comment,\";comment\".\n" +
            "comment,\";abc\".\n" +
            "lparen.\n" +
            "comment,\";\".\n" +
            "identifier,\"plain\".\n";
    
        Printtokens.main(args);
        String actualOutput = outContent.toString().trim();
        assertEquals(expectedOutput.trim(), actualOutput);
    }

    @Test
    public void testOpenTokenStreamBranches(@TempDir Path tempDir) throws IOException {
        String input = "and\n";
        InputStream in = new ByteArrayInputStream(input.getBytes());
        System.setIn(in);
        BufferedReader brNull = pt.open_token_stream(null);
        String token = pt.get_token(brNull);
        pt.print_token(token);
        String actualOutput = outContent.toString().trim();
        assertEquals("keyword,\"and\".", actualOutput);
        outContent.reset();

        Path inputFile = createTempFile(tempDir, "test_open_stream", "or\n");
        BufferedReader brFile = pt.open_token_stream(inputFile.toString());
        token = pt.get_token(brFile);
        pt.print_token(token);
        actualOutput = outContent.toString().trim();
        assertEquals("keyword,\"or\".", actualOutput);
    }

    @Test
    public void testGetTokenSemicolonBranch(@TempDir Path tempDir) throws IOException {
        Path inputFile = createTempFile(tempDir, "test_get_token_semicolon", "abc;\n");
        String[] args = {inputFile.toString()};
        String expectedOutput = 
            "identifier,\"abc\".\n" +
            "comment,\";\".";
        runMainAndAssert(args, expectedOutput);
    }

    @Test
    public void testMainMultipleArgsError() {
        System.setIn(new ByteArrayInputStream(new byte[0]));
        String[] args = {"file1", "file2"};
        Printtokens.main(args);
        String actualOutput = outContent.toString().trim();
        String expectedOutput = "Error! Please give the token stream";
        assertEquals(expectedOutput, actualOutput);
    }
}