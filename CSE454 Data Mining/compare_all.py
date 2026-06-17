import time
import pandas as pd
import matplotlib.pyplot as plt
import utils  # utils.py dosyanızdaki start/stop_memory_profile fonksiyonlarını kullanır

# Manuel Implementasyonlar
from fptree import create_tree, mine_tree, convert_to_init_set, load_data
from apriori import apriori as manual_apriori

# Kütüphane İmplementasyonları
from mlxtend.preprocessing import TransactionEncoder
from mlxtend.frequent_patterns import apriori as mlxtend_apriori
from mlxtend.frequent_patterns import fpgrowth as mlxtend_fpgrowth

# ==========================================
# CONFIGURATION
# ==========================================
INPUT_FILE = 'online_retail_II.csv'
MIN_SUPPORT = 300  # Minimum destek eşiği (İşlem sayısı)

def benchmark():
    # 1. Veri Yükleme
    print(f"--- Loading Data: {INPUT_FILE} ---")
    dataset = load_data(INPUT_FILE)
    if not dataset:
        print("Dataset could not be loaded. Please check the file path.")
        return

    num_transactions = len(dataset)
    min_sup_ratio = MIN_SUPPORT / num_transactions
    results = []

    # 2. Kütüphane Verisi Hazırlama (One-Hot Encoding)
    # Bellek kullanımını düşürmek için 'sparse' matris ve 'bool' tipi kullanılır.
    print("Preparing Library Data (Sparse One-Hot Matrix)...")
    te = TransactionEncoder()
    te_ary = te.fit(dataset).transform(dataset, sparse=True)
    # Optimizasyon: dtype='bool' RAM kullanımını ciddi oranda azaltır
    df = pd.DataFrame.sparse.from_spmatrix(te_ary, columns=te.columns_).astype(bool)

    # --- BENCHMARK 1: MANUAL FP-GROWTH ---
    print("\n[1/4] Running: Manual FP-Growth...")
    init_set = convert_to_init_set(dataset)
    utils.start_memory_profile()
    start_time = time.time()
    
    my_fp, my_header = create_tree(init_set, MIN_SUPPORT)
    freq_items = []
    if my_fp:
        mine_tree(my_fp, my_header, MIN_SUPPORT, set([]), freq_items)
        
    duration = time.time() - start_time
    mem_usage = utils.stop_memory_profile() / 10**6  # Byte to MB
    results.append({'Algorithm': 'Manual FP-Growth', 'Time (s)': duration, 'Memory (MB)': mem_usage})

    # --- BENCHMARK 2: LIBRARY FP-GROWTH ---
    print("[2/4] Running: Library FP-Growth...")
    utils.start_memory_profile()
    start_time = time.time()
    
    # use_colnames=True ek yük bindirir ancak sonuç okunabilirliği için açık bırakıldı
    mlxtend_fpgrowth(df, min_support=min_sup_ratio, use_colnames=True)
    
    duration = time.time() - start_time
    mem_usage = utils.stop_memory_profile() / 10**6
    results.append({'Algorithm': 'Library FP-Growth', 'Time (s)': duration, 'Memory (MB)': mem_usage})

    # --- BENCHMARK 3: MANUAL APRIORI ---
    print("[3/4] Running: Manual Apriori...")
    utils.start_memory_profile()
    start_time = time.time()
    
    manual_apriori(dataset, MIN_SUPPORT)
    
    duration = time.time() - start_time
    mem_usage = utils.stop_memory_profile() / 10**6
    results.append({'Algorithm': 'Manual Apriori', 'Time (s)': duration, 'Memory (MB)': mem_usage})

    # --- BENCHMARK 4: LIBRARY APRIORI ---
    print("[4/4] Running: Library Apriori...")
    utils.start_memory_profile()
    start_time = time.time()
    
    mlxtend_apriori(df, min_support=min_sup_ratio, use_colnames=True)
    
    duration = time.time() - start_time
    mem_usage = utils.stop_memory_profile() / 10**6
    results.append({'Algorithm': 'Library Apriori', 'Time (s)': duration, 'Memory (MB)': mem_usage})

    # --- SONUÇLAR VE GÖRSELLEŞTİRME ---
    res_df = pd.DataFrame(results)
    print("\n" + "="*30)
    print("FINAL BENCHMARK RESULTS")
    print("="*30)
    print(res_df)

    # Grafik Çizimi
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))
    
    # Zaman Grafiği
    res_df.plot(x='Algorithm', y='Time (s)', kind='bar', ax=ax1, color=['#3498db', '#2980b9', '#e74c3c', '#c0392b'], legend=False)
    ax1.set_title('Execution Time Comparison', fontweight='bold')
    ax1.set_ylabel('Time (Seconds)')
    ax1.set_xticklabels(res_df['Algorithm'], rotation=45)
    ax1.grid(axis='y', linestyle='--', alpha=0.7)

    # Bellek Grafiği
    res_df.plot(x='Algorithm', y='Memory (MB)', kind='bar', ax=ax2, color=['#2ecc71', '#27ae60', '#f1c40f', '#f39c12'], legend=False)
    ax2.set_title('Peak Memory Usage Comparison', fontweight='bold')
    ax2.set_ylabel('Memory (MB)')
    ax2.set_xticklabels(res_df['Algorithm'], rotation=45)
    ax2.grid(axis='y', linestyle='--', alpha=0.7)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    benchmark()