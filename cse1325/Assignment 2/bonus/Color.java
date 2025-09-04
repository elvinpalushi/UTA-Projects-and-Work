public enum Color{CHARTREUSE(0x80FF00), ISABELLINE(0xF4F0EC), LILAC(0xC8A2C8), PEACH(0xFFE5B4);

    private final int rgb;

    private Color(int rgb) {
        this.rgb = rgb;
    }

    @Override
    public String toString() {
        return String.format("%10s (0x%X)", name(), rgb);
    }
}