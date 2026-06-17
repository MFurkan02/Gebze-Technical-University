import csv
import time
import os
from itertools import combinations
import utils 

# ==========================================
# SETTINGS
# ==========================================
INPUT_FILE = 'online_retail_II.csv'
OUTPUT_FILE = 'results_apriori.txt'
MIN_SUPPORT = 500

# ==========================================
# 1. HELPER FUNCTIONS
# ==========================================
def create_C1(dataset):
    C1 = []
    for transaction in dataset:
        for item in transaction:
            if [item] not in C1:
                C1.append([item])
    C1.sort()
    return list(map(frozenset, C1))

def scan_D(dataset, candidates, min_sup):
    ss_cnt = utils.parallel_scan(dataset, candidates)
    
    ret_list = []
    support_data = {}
    for key in ss_cnt:
        support = ss_cnt[key]
        if support >= min_sup:
            ret_list.insert(0, key)
        support_data[key] = support
    return ret_list, support_data

def apriori_gen(Lk, k):
    ret_list = []
    len_Lk = len(Lk)
    for i in range(len_Lk):
        for j in range(i + 1, len_Lk):
            L1 = list(Lk[i])[:k - 2]
            L2 = list(Lk[j])[:k - 2]
            L1.sort()
            L2.sort()
            if L1 == L2:
                ret_list.append(Lk[i] | Lk[j])
    return ret_list

def apriori(dataset, min_sup):
    C1 = create_C1(dataset)
    D = list(map(set, dataset))
    L1, support_data = scan_D(D, C1, min_sup)
    L = [L1]
    k = 2
    while (len(L[k - 2]) > 0):
        Ck = apriori_gen(L[k - 2], k)
        Lk, supK = scan_D(D, Ck, min_sup)
        support_data.update(supK)
        L.append(Lk)
        k += 1
    return L, support_data

def load_data(filename):
    if not os.path.exists(filename):
        print(f"ERROR: '{filename}' not found!")
        return None
    print(f"Reading file [{filename}]...")
    from collections import defaultdict
    transactions = defaultdict(list)
    noise = ['POSTAGE', 'DOT', 'M', 'D', 'S', 'BANK CHARGES', 'PADS', 'ADJUST', 'CRUK']
    try:
        with open(filename, 'r', encoding='utf-8-sig', errors='replace') as f:
            reader = csv.reader(f)
            next(reader, None)
            for row in reader:
                if len(row) < 3: continue
                inv, code, desc = row[0].strip(), row[1].strip(), row[2].strip()
                if inv.startswith('C') or code in noise or not desc: continue
                transactions[inv].append(desc)
    except Exception as e:
        print(f"Error: {e}")
        return None
    return list(transactions.values())

# ==========================================
# 2. MAIN
# ==========================================
if __name__ == "__main__":
    dataset = load_data(INPUT_FILE)
    
    if dataset:
        print(f"\n--- STARTING APRIORI (Min Sup: {MIN_SUPPORT}) ---")
        
        # --- TEST 1: PARALLELISM TEST ---
        print("\n[TEST 1] Parallel Performance Test (Multiprocessing)")
        test_candidates = [frozenset([x]) for x in set(x for t in dataset for x in t)][:1000]
        
        t1 = time.time()
        utils.parallel_scan(dataset, test_candidates, num_cores=1)
        single_core_time = time.time() - t1
        print(f"-> Single Core : {single_core_time:.4f} sec")
        
        t2 = time.time()
        utils.parallel_scan(dataset, test_candidates)
        multi_core_time = time.time() - t2
        print(f"-> Multi Core  : {multi_core_time:.4f} sec")

        # --- TEST 2: SENSITIVITY ANALYSIS ---
        print("\n[TEST 2] Sensitivity Analysis (Stress Test)")
        print("Measuring algorithm response at different 'Support' values...")
        
        test_supports = [500, 400, 300] 
        results_rows = []
        L_final_patterns = [] # Added for logging
        
        print("-" * 50)
        print(f"{'Min Support':<15} | {'Pattern Count':<15} | {'Time (s)':<10}")
        print("-" * 50)
        
        for sup in test_supports:
            t_start = time.time()
            try:
                # Modified to capture support_data
                L_test, sup_data_local = apriori(dataset, sup) 
                pattern_count = sum(len(level) for level in L_test)
                duration = time.time() - t_start
                
                print(f"{sup:<15} | {pattern_count:<15} | {duration:.4f}")
                results_rows.append((sup, pattern_count, duration))
                
                if sup == MIN_SUPPORT:
                    for level in L_test:
                        for itemset in level:
                            count = sup_data_local.get(itemset, 0)
                            L_final_patterns.append((list(itemset), count))
                            
            except Exception as e:
                print(f"{sup:<15} | ERROR: {e}")
                results_rows.append((sup, "ERROR", 0))

        # --- FILE WRITING BLOCK ---
        print(f"\nWriting detailed report to '{OUTPUT_FILE}'...")
        try:
            with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
                f.write("==================================================\n")
                f.write("          APRIORI ALGORITHM PERFORMANCE REPORT    \n")
                f.write("==================================================\n\n")
                
                f.write(f"Dataset      : {INPUT_FILE}\n\n")
                
                f.write("[TEST 1] PARALLEL PROCESSING BENCHMARK\n")
                f.write("-" * 50 + "\n")
                f.write(f"Single Core Time : {single_core_time:.4f} sec\n")
                f.write(f"Multi Core Time  : {multi_core_time:.4f} sec\n")
                speedup = single_core_time / multi_core_time if multi_core_time > 0 else 0
                f.write(f"Speedup Factor   : {speedup:.2f}x\n\n")
                
                f.write("[TEST 2] SENSITIVITY ANALYSIS (STRESS TEST)\n")
                f.write("-" * 50 + "\n")
                f.write(f"{'Min Support':<15} | {'Patterns Found':<15} | {'Time (s)':<10}\n")
                f.write("-" * 50 + "\n")
                
                for row in results_rows:
                    f.write(f"{row[0]:<15} | {row[1]:<15} | {row[2]:.4f}\n")
                
                f.write("\n\n")
                f.write(f"[FOUND PATTERNS (Min Support: {MIN_SUPPORT})]\n")
                f.write("-" * 50 + "\n")
                L_final_patterns.sort(key=lambda x: x[1], reverse=True)
                for p, count in L_final_patterns:
                    f.write(f"Count: {count:<6} | Items: {p}\n")
            
            print(" -> Report successfully saved!")
        except Exception as e:
            print(f" -> ERROR writing file: {e}")