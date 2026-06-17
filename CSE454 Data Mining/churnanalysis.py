import csv
import sys
import os
import time
from collections import defaultdict, Counter
import utils 

# ==========================================
# SETTINGS
# ==========================================
INPUT_FILE = 'online_retail_II.csv'
OUTPUT_FILE = 'results_churn_analysis.txt'
MIN_SALES_THRESHOLD = 50  # Minimum sales required to calculate return rate (to avoid noise)

# ==========================================
# 1. DATA LOADING (CUSTOM PARSER)
# ==========================================
def load_and_split_data(filename):
    if not os.path.exists(filename):
        print(f"ERROR: '{filename}' not found!")
        sys.exit(1)
        
    print(f"Reading and parsing file [{filename}]...")
    
    sales_transactions = defaultdict(list)   # Normal Sales
    return_transactions = defaultdict(list)  # Cancellations (Starts with C)
    
    noise_codes = ['POSTAGE', 'DOT', 'M', 'D', 'S', 'BANK CHARGES', 'PADS', 'ADJUST', 'CRUK']
    
    try:
        with open(filename, 'r', encoding='utf-8-sig', errors='replace') as csvfile:
            reader = csv.reader(csvfile)
            next(reader, None)
            
            for row in reader:
                if len(row) < 3: continue
                inv, code, desc = row[0].strip(), row[1].strip(), row[2].strip()
                
                if code in noise_codes or not desc: continue
                
                if inv.startswith('C'):
                    return_transactions[inv].append(desc)
                else:
                    sales_transactions[inv].append(desc)
                    
    except Exception as e:
        print(f"Error: {e}")
        return None, None

    print(f"  -> {len(sales_transactions)} Sales Invoices found.")
    print(f"  -> {len(return_transactions)} Return Invoices found.")
    
    return list(sales_transactions.values()), list(return_transactions.values())

# ==========================================
# 2. MAIN ANALYSIS
# ==========================================
if __name__ == "__main__":
    sales, returns = load_and_split_data(INPUT_FILE)
    
    if sales and returns:
        
        # --- ANALYSIS 1: RETURN RATES ---
        print("\n[ANALYSIS 1] RETURN RATES (Most Problematic Products)")
        
        # Count all sales and returns
        all_sales_items = [item for sublist in sales for item in sublist]
        all_return_items = [item for sublist in returns for item in sublist]
        
        sales_counts = Counter(all_sales_items)
        return_counts = Counter(all_return_items)
        
        product_risk = []
        
        for product, s_count in sales_counts.items():
            if s_count > MIN_SALES_THRESHOLD:
                r_count = return_counts.get(product, 0)
                return_rate = (r_count / s_count) * 100
                product_risk.append((product, return_rate, s_count, r_count))
        
        # Sort by return rate
        product_risk.sort(key=lambda x: x[1], reverse=True)
        
        print(f"Most Returned Products (Top 10):")
        print(f"{'Product':<40} | {'Rate %':<10} | {'Sales':<8} | {'Returns':<8}")
        print("-" * 80)
        for p in product_risk[:10]:
            print(f"{p[0][:38]:<40} | %{p[1]:<9.1f} | {p[2]:<8} | {p[3]:<8}")

        # --- ANALYSIS 2: CO-RETURNED 'TOXIC' BUNDLES ---
        print("\n[ANALYSIS 2] Co-Returned 'Toxic' Bundles")
        print("Finding products people return together...")
        
        # Since returns are sparse, we keep Min Support very low
        MIN_RETURN_SUPPORT = 5 
        
        # Manual Apriori candidate counting
        c1 = [frozenset([item]) for item in set(item for trans in returns for item in trans)]
        l1_counts = utils.parallel_scan(returns, c1, num_cores=4)
        
        # Filter by support
        frequent_returns = {k: v for k, v in l1_counts.items() if v >= MIN_RETURN_SUPPORT}
        
        sorted_returns = sorted(frequent_returns.items(), key=lambda x: x[1], reverse=True)
        
        print(f"\nMost Frequently Returned Single Items (Top 5):")
        for item, count in sorted_returns[:5]:
            print(f"  -> {list(item)[0]} (Return Count: {count})")
            
        print(f"\nDetailed report writing to '{OUTPUT_FILE}'...")