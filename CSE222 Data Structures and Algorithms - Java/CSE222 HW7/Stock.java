public class Stock {
    private String symbol;
    private double price;
    private long volume;
    private long marketCap;
    
    /**
     * Constructor of Stock.
     * @param symbol
     * @param price
     * @param volume
     * @param marketCap
     */
    public Stock(String symbol , double price , long volume , long marketCap){
        this.symbol = symbol ;
        this.price = price ;
        this.volume = volume ;
        this.marketCap = marketCap;
    }
    
    /**
     * Method for printing the attributes of stock.
     */
    public String printStockInfo(){
        return "Symbol -> " + symbol + " Price -> " + price + " Volume -> " + volume + " MarketCap -> " + marketCap ;
    }

    
    /**
     * Getter for getting the symbol string.
     * @return
     */
    public String getSymbol() {
        return symbol;
    }

    // Setter for symbol
    public void setSymbol(String symbol) {
        this.symbol = symbol;
    }

    // Getter for price
    public double getPrice() {
        return price;
    }

    // Setter for price
    public void setPrice(double price) {
        this.price = price;
    }

    // Getter for volume
    public long getVolume() {
        return volume;
    }

    // Setter for volume
    public void setVolume(long volume) {
        this.volume = volume;
    }

    // Getter for marketCap
    public long getMarketCap() {
        return marketCap;
    }

    // Setter for marketCap
    public void setMarketCap(long marketCap) {
        this.marketCap = marketCap;
    }
}
