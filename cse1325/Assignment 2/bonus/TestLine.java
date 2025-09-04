public class TestLine {
    public static void main(String[] args) {
        for(Color color : Color.values()) {

            double x1 = Math.random() * 9 + 1;
            double y1 = Math.random() * 9 + 1;
            double x2 = Math.random() * 9 + 1;
            double y2 = Math.random() * 9 + 1;

            Line line = new Line(x1, y1, x2, y2, color);

            System.out.println(line + " has length " + line.length());
        }
    }
}