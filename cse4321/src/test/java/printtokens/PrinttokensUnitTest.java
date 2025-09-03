package printtokens;

import org.junit.jupiter.api.*;
import static org.junit.jupiter.api.Assertions.*;
import java.io.*;
import org.junit.jupiter.api.io.TempDir;
import java.nio.file.Files;
import java.nio.file.Path;

public class PrinttokensUnitTest {
    private Printtokens pt;

    @BeforeEach
    public void setUp() {
        pt = new Printtokens();
    }

    // // Test 1: Main
    // @Test
    // public void testMainWithFileInput() {
    //     String[] args = {"src/test/java/printtokens/test1.txt"};
    //     Printtokens.main(args);
    // }

    // @Test
    // public void testMainWithKeywordInput() {
    //     ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    //     System.setOut(new PrintStream(outContent));

    //     String input = "and\n";
    //     InputStream in = new ByteArrayInputStream(input.getBytes());
    //     System.setIn(in);

    //     String[] args = {};
    //     Printtokens.main(args);

    //     String output = outContent.toString().trim();
    //     assertEquals("keyword,\"and\".", output);

    //     System.setIn(System.in);
    //     System.setOut(System.out);
    // }

    // @Test
    // void testMain_WithTooManyArguments() {
    //     ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    //     System.setOut(new PrintStream(outContent));

    //     Printtokens.main(new String[]{"file1.txt", "file2.txt"});
    
    //     assertTrue(outContent.toString().contains("Error! Please give the token stream"));
    // }

    // Test 2: open_character_stream
    @Test
    public void testOpenCharacterStreamWithFile() throws IOException {
        BufferedReader br = pt.open_character_stream("src/test/java/printtokens/test1.txt");
        assertNotNull(br);
        assertEquals("if (x < 5)", br.readLine());
        br.close();
    }

    @Test
    public void testOpenCharacterStreamWithNonexistentFile() throws IOException {
        ByteArrayOutputStream outContent = new ByteArrayOutputStream();
        System.setOut(new PrintStream(outContent));
    
        BufferedReader br = pt.open_character_stream("test2.txt");
    
        assertNull(br);
    
        String output = outContent.toString().trim();
        assertTrue(output.contains("The file test2.txt doesn't exists"));
    
        System.setOut(System.out);
    }

    // Get BufferedReader
    private static class ThrowingBufferedReader extends BufferedReader {
        ThrowingBufferedReader() {
            super(new StringReader(""));
        }
    
        @Override
        public void mark(int readAheadLimit) throws IOException {
            throw new IOException("Test IOException for mark");
        }
    
        @Override
        public int read() throws IOException {
            throw new IOException("Test IOException for read");
        }
    
        @Override
        public void reset() throws IOException {
            throw new IOException("Test IOException for reset");
        }
    }

    // Test 3: get_char
    @Test
    public void testGetChar() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("a"));
        assertEquals(97, pt.get_char(br));
    }

    @Test
    public void testGetCharIOException() {
        BufferedReader br = new ThrowingBufferedReader();
        int result = pt.get_char(br);
        assertEquals(0, result);
    }

    // Test 4: unget_char
    @Test
    public void testUngetChar() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("b"));
        br.mark(4);
        pt.get_char(br);
        assertEquals(0, pt.unget_char('b', br));
    }

    @Test
    public void testUngetCharIOException() {
        BufferedReader br = new ThrowingBufferedReader();
        char result = pt.unget_char('a', br);
        assertEquals(0, result);
    }

    // Test 5: open_token_stream
    @Test
    public void testOpenTokenStreamEmptyInput() {
        BufferedReader br = pt.open_token_stream("");
        assertNotNull(br);
    }

    @Test
    public void testOpenTokenStreamWithFile() {
        BufferedReader br = pt.open_token_stream("src/test/java/printtokens/test1.txt");
        assertNotNull(br);
    }

    @Test
    public void testOpenTokenStreamBranches(@TempDir Path tempDir) throws IOException {
        String input = "and\n";
        ByteArrayInputStream in = new ByteArrayInputStream(input.getBytes());
        System.setIn(in);
        BufferedReader brNull = pt.open_token_stream(null);
        assertEquals("and", brNull.readLine());

        Path inputFile = Files.createTempFile(tempDir, "test_open_stream", ".txt");
        Files.writeString(inputFile, "or\n");
        BufferedReader brFile = pt.open_token_stream(inputFile.toString());
        assertEquals("or", brFile.readLine());
    }

    // Test 6: get_token
    @Test
    public void testGetTokenEmptyInput() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader(""));
        assertNull(pt.get_token(br));
    }

    @Test
    public void testGetTokenWithSpecialSymbol() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("("));
        assertEquals("(", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithMultipleSpecialSymbols() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("( ) [ ]"));
        assertEquals("(", pt.get_token(br));
        assertEquals(")", pt.get_token(br));
        assertEquals("[", pt.get_token(br));
        assertEquals("]", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithString() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("\"hello\""));
        assertEquals("\"hello\"", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithUnclosedString() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("\"hello"));
        assertEquals("\"hello", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithComment() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader(";comment\n"));
        assertEquals(";comment", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithNumber() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("123"));
        assertEquals("123", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithIdentifier() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("variable1"));
        assertEquals("variable1", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithMixedInput() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("if (x > 5)"));
        assertEquals("if", pt.get_token(br));
        assertEquals("(", pt.get_token(br));
        assertEquals("x", pt.get_token(br));
        assertEquals(">", pt.get_token(br));
        assertEquals("5", pt.get_token(br));
        assertEquals(")", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithCharConstant() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("#a"));
        assertEquals("#a", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithSemicolon() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader(";"));
        assertEquals(";", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithEOFAfterToken() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("hello"));
        assertEquals("hello", pt.get_token(br));
        assertNull(pt.get_token(br));
    }

    @Test
    public void testGetTokenWithMultipleLines() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("first\nsecond"));
        assertEquals("first", pt.get_token(br));
        assertEquals("second", pt.get_token(br));
    }

    @Test
    public void testGetTokenWithComplexExpression() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("if (x > 5)"));
        assertEquals("if", pt.get_token(br));
        assertEquals("(", pt.get_token(br));
        assertEquals("x", pt.get_token(br));
        assertEquals(">", pt.get_token(br));
        assertEquals("5", pt.get_token(br));
        assertEquals(")", pt.get_token(br));
    }

    @Test
    public void testGetTokenSemicolonAfterIdentifier() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("abc;\n"));
        assertEquals("abc", pt.get_token(br));
        assertEquals(";", pt.get_token(br));
        assertNull(pt.get_token(br));
    }

    @Test
    public void testGetTokenEOFAfterFirstChar() throws IOException {
        BufferedReader br = new BufferedReader(new StringReader("("));
        assertEquals("(", pt.get_token(br));
        assertNull(pt.get_token(br));
    }

    // Test 7: is_token_end
    @Test
    public void testIsTokenTrue() {
        assertTrue(Printtokens.is_token_end(0, -1));
        assertTrue(Printtokens.is_token_end(1, 10));
        assertTrue(Printtokens.is_token_end(2, 10));
        assertTrue(Printtokens.is_token_end(0, 40));
        assertTrue(Printtokens.is_token_end(0, 59));

        assertTrue(Printtokens.is_token_end(1, 34));
        assertTrue(Printtokens.is_token_end(1, 13));
        assertTrue(Printtokens.is_token_end(1, 9));
        assertTrue(Printtokens.is_token_end(2, 13));
        assertTrue(Printtokens.is_token_end(2, 9));
        assertTrue(Printtokens.is_token_end(0, 13));
        assertTrue(Printtokens.is_token_end(0, 10));
        assertTrue(Printtokens.is_token_end(0, 32));
    }

    @Test
    public void testIsTokenFalse() {
        assertFalse(Printtokens.is_token_end(1, 11));
        assertFalse(Printtokens.is_token_end(2, 11));
        assertFalse(Printtokens.is_token_end(0, 60));

        assertFalse(Printtokens.is_token_end(2, 11));
    }

    // Test 8: token_type
    @Test
    public void testTokenTypeKeyword() {
        assertEquals(Printtokens.keyword, Printtokens.token_type("and"));
    }

    @Test
    public void testTokenTypeSpecialSymbol() {
        assertEquals(Printtokens.spec_symbol, Printtokens.token_type("("));
    }

    @Test
    public void testTokenTypeIdentifier() {
        assertEquals(Printtokens.identifier, Printtokens.token_type("a1"));
    }

    @Test
    public void testTokenTypeNumConstant() {
        assertEquals(Printtokens.num_constant, Printtokens.token_type("123"));
    }

    @Test
    public void testTokenTypeStrConstant() {
        assertEquals(Printtokens.str_constant, Printtokens.token_type("\"asd\""));
    }

    @Test
    public void testTokenTypeCharConstant() {
        assertEquals(Printtokens.char_constant, Printtokens.token_type("#a"));
    }

    @Test
    public void testTokenTypeComment() {
        assertEquals(Printtokens.comment, Printtokens.token_type(";comment"));
    }

    @Test
    public void testTokenTypeError() {
        assertEquals(Printtokens.error, Printtokens.token_type("112A"));
    }

    // Test 9: print_token
    @Test
    public void testPrintTokenError() {
        testPrintTokenOutput("112A", "error,\"112A\".");
    }

    @Test
    public void testPrintTokenKeyword() {
        testPrintTokenOutput("and", "keyword,\"and\".");
        testPrintTokenOutput("or", "keyword,\"or\".");
        testPrintTokenOutput("if", "keyword,\"if\".");
    }

    @Test
    public void testPrintTokenSpecialSymbol() {
        testPrintTokenOutput("(", "lparen.");
        testPrintTokenOutput(")", "rparen.");
        testPrintTokenOutput("[", "lsquare.");
    }

    @Test
    public void testPrintTokenIdentifier() {
        testPrintTokenOutput("variable1", "identifier,\"variable1\".");
        testPrintTokenOutput("x", "identifier,\"x\".");
    }

    @Test
    public void testPrintTokenNumConstant() {
        testPrintTokenOutput("123", "numeric,123.");
        testPrintTokenOutput("0", "numeric,0.");
    }

    @Test
    public void testPrintTokenStrConstant() {
        testPrintTokenOutput("\"hello\"", "string,\"hello\".");
        testPrintTokenOutput("\"\"", "string,\"\".");
    }

    @Test
    public void testPrintTokenCharConstant() {
        testPrintTokenOutput("#a", "character,\"a\".");
        testPrintTokenOutput("#Z", "character,\"Z\".");
    }

    @Test
    public void testPrintTokenComment() {
        testPrintTokenOutput(";comment", "comment,\";comment\".");
        testPrintTokenOutput(";;", "comment,\";;\".");
    }

    private void testPrintTokenOutput(String token, String expected) {
        ByteArrayOutputStream outContent = new ByteArrayOutputStream();
        System.setOut(new PrintStream(outContent));
    
        pt.print_token(token);
    
        String actual = outContent.toString().trim();
        assertEquals(expected, actual);
    
        System.setOut(System.out);
    }

    // Test 10: is_comment
    @Test
    public void testIsCommentTrue() {
        assertTrue(Printtokens.is_comment(";comment"));
    }

    @Test
    public void testIsCommentFalse() {
        assertFalse(Printtokens.is_comment("comment"));
    }

    // Test 11: is_keyword
    @Test
    public void testIsKeywordTrue() {
        assertTrue(Printtokens.is_keyword("and"));
        assertTrue(Printtokens.is_keyword("or"));
        assertTrue(Printtokens.is_keyword("if"));
        assertTrue(Printtokens.is_keyword("xor"));
        assertTrue(Printtokens.is_keyword("lambda"));
        assertTrue(Printtokens.is_keyword("=>"));
    }

    @Test
    public void testIsKeywordFalse() {
        assertFalse(Printtokens.is_keyword("maybe"));
    }

    // Test 12: is_char_constant
    @Test
    public void testIsCharConstantTrue() {
        assertTrue(Printtokens.is_char_constant("#a"));
    }

    @Test
    public void testIsCharConstantFalse() {
        assertFalse(Printtokens.is_char_constant("a"));
    }

    @Test
    public void testIsCharConstantBranches() {
        assertTrue(Printtokens.is_char_constant("#a"));
        assertFalse(Printtokens.is_char_constant("abc"));
        assertFalse(Printtokens.is_char_constant("#1"));
    }

    // Test 13: is_num_constant
    @Test
    public void testIsNumConstantTrue() {
        assertTrue(Printtokens.is_num_constant("234"));
    }

    @Test
    public void testIsNumConstantFalse() {
        assertFalse(Printtokens.is_num_constant("B"));
        assertFalse(Printtokens.is_num_constant("2B"));
    }

    // Test 14: is_str_constant
    @Test
    public void testIsStrConstantTrue() {
        assertTrue(Printtokens.is_str_constant("\"abc\""));
    }

    @Test
    public void testIsStrConstantFalse() {
        assertFalse(Printtokens.is_str_constant("abc\""));
        assertFalse(Printtokens.is_str_constant("\"abc"));
    }

    @Test
    public void testIsStrConstantBranches() {
        assertTrue(Printtokens.is_str_constant("\"hello\""));
        assertFalse(Printtokens.is_str_constant("\"abc"));
        assertFalse(Printtokens.is_str_constant("abc"));
    }

    // Test 15: is_identifier
    @Test
    public void testIsIdentifierTrue() {
        assertTrue(Printtokens.is_identifier("a123"));
    }

    @Test
    public void testIsIdentifierFalse() {
        assertFalse(Printtokens.is_identifier("1a"));
        assertFalse(Printtokens.is_identifier("aa!"));
    }

    @Test
    public void testIsIdentifierBranches() {
        assertTrue(Printtokens.is_identifier("abc123"));
        assertFalse(Printtokens.is_identifier("ab!"));
        assertFalse(Printtokens.is_identifier("123"));
    }

    // Test 16: print_spec_symbol
    @Test
    public void testPrintSpecSymbol() {
        testPrintSymbolOutput("(", "lparen.");
        testPrintSymbolOutput(")", "rparen.");
        testPrintSymbolOutput("[", "lsquare.");
        testPrintSymbolOutput("]", "rsquare.");
        testPrintSymbolOutput("'", "quote.");
        testPrintSymbolOutput("`", "bquote.");
        testPrintSymbolOutput(",", "comma.");
    }

    @Test
    public void testPrintInvalidSpecSymbol() {
        testPrintSymbolOutput("?", "");
    }

    private void testPrintSymbolOutput(String symbol, String expected) {
        ByteArrayOutputStream outContent = new ByteArrayOutputStream();
        System.setOut(new PrintStream(outContent));
    
        Printtokens.print_spec_symbol(symbol);
    
        String actual = outContent.toString().trim();
        assertEquals(expected, actual);
    
        System.setOut(System.out);
    }

    // Test 17: is_spec_symbol
    @Test
    public void testIsSpecSymbolTrue() {
        assertTrue(Printtokens.is_spec_symbol('('));
        assertTrue(Printtokens.is_spec_symbol(')'));
        assertTrue(Printtokens.is_spec_symbol('['));
        assertTrue(Printtokens.is_spec_symbol(']'));
        assertTrue(Printtokens.is_spec_symbol('\''));
        assertTrue(Printtokens.is_spec_symbol('`'));
        assertTrue(Printtokens.is_spec_symbol(','));
    }

    @Test
    public void testIsSpecSymbolFalse() {
        assertFalse(Printtokens.is_spec_symbol('?'));
    }
}