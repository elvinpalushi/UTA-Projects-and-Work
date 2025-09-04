public enum Color{CHARTREUSE(0x80FF00), ISABELLINE(0xF4F0EC), LILAC(0xC8A2C8), PEACH(0xFFE5B4);

    private final int rgb;

    private Color(int rgb) {
        this.rgb = rgb;
    }

    @Override
    public String toString() {
        return String.format("%s %10s (%s0x%X%s)", resetColor(), name(), asColor(), rgb, resetColor());
    }

    public String asColor() {
        return String.format("\033[38;2;%d;%d;%dm", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
    }

    public static String resetColor() {
        return "\033[0m";
    }

}