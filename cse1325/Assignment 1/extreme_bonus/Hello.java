public class Hello {
    public static void main(String[] args) {
        String identifier = System.getProperty("user.name");
        System.out.println("Hello, " + identifier + "!");
    }
}