import multiprocessing
import tracemalloc
from itertools import combinations

# ==========================================
# 1. PERFORMANCE TOOLS
# ==========================================
def start_memory_profile():
    tracemalloc.start()

def stop_memory_profile():
    current, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    return peak

def _worker_count(data_chunk, candidates):
    local_counts = {}
    for transaction in data_chunk:
        trans_set = set(transaction)
        for can in candidates:
            if can.issubset(trans_set):
                local_counts[can] = local_counts.get(can, 0) + 1
    return local_counts

def parallel_scan(dataset, candidates, num_cores=None):
    if num_cores is None: num_cores = multiprocessing.cpu_count()
    chunk_size = len(dataset) // num_cores if len(dataset) // num_cores > 0 else 1
    chunks = [dataset[i:i + chunk_size] for i in range(0, len(dataset), chunk_size)]
    
    pool = multiprocessing.Pool(processes=num_cores)
    results = []
    
    for chunk in chunks:
        res = pool.apply_async(_worker_count, (chunk, candidates))
        results.append(res)
        
    pool.close()
    pool.join()
    
    total_counts = {}
    for res in results:
        local_dict = res.get()
        for key, val in local_dict.items():
            total_counts[key] = total_counts.get(key, 0) + val
    return total_counts

# ==========================================
# 2. ADVANCED ANALYSIS (LIFT & LEVERAGE)
# ==========================================
def generate_advanced_rules(support_data, total_transactions, min_conf=0.1, min_lift=1.0):
    rules = []
    
    for itemset, support_count in support_data.items():
        if len(itemset) > 1:
            support_ratio_union = support_count / total_transactions
            
            for i in range(1, len(itemset)):
                for antecedent in combinations(itemset, i):
                    antecedent = frozenset(antecedent)
                    consequent = itemset - antecedent
                    
                    if antecedent in support_data and consequent in support_data:
                        sup_ant = support_data[antecedent]
                        sup_cons = support_data[consequent]
                        
                        prob_ant = sup_ant / total_transactions
                        prob_cons = sup_cons / total_transactions
                        
                        confidence = support_ratio_union / prob_ant
                        
                        # Safety Lock: Confidence cannot exceed 100%
                        if confidence > 1.0: confidence = 1.0
                        
                        lift = confidence / prob_cons
                        leverage = support_ratio_union - (prob_ant * prob_cons)
                        
                        if confidence >= min_conf and lift >= min_lift:
                            rules.append({
                                'ant': list(antecedent),
                                'cons': list(consequent),
                                'conf': confidence,
                                'lift': lift,
                                'lev': leverage,
                                'support_count': support_count 
                            })
    
    rules.sort(key=lambda x: x['lift'], reverse=True)
    return rules

# ==========================================
# 3. RECOMMENDATION ENGINE
# ==========================================
def print_recommendations(rules, basket, top_n=5):
    print(f"\n[ADVANCED ANALYSIS] Basket Content: {basket}")
    basket_set = set(basket)
    
    matches = [r for r in rules if set(r['ant']).issubset(basket_set)]
    
    if matches and 'support_count' in matches[0]:
        matches.sort(key=lambda x: x['lift'] * (x['support_count']**0.5), reverse=True)
    else:
        matches.sort(key=lambda x: x['lift'], reverse=True)
    
    if not matches:
        print("  -> No strong rule found for this basket.")
        return

    print("\n  >>> RECOMMENDED ITEMS (Set Completion)")
    
    count = 0
    seen = set()
    
    for r in matches:
        if count >= top_n: break
        recs = [item for item in r['cons'] if item not in basket_set]
        
        for item in recs:
            if item not in seen:
                print(f"      * {item}")
                pop_str = f" | Popularity: {r['support_count']}" if 'support_count' in r else ""
                print(f"        (Confidence: %{r['conf']*100:.1f} | Lift: {r['lift']:.2f}x | Leverage: {r['lev']:.4f}{pop_str})")
                seen.add(item)
                count += 1
                if count >= top_n: break

# ==========================================
# 4. SPURIOUS PATTERN DETECTION
# ==========================================
def detect_coincidental_patterns(support_data, total_transactions):
    coincidences = []
    for itemset, support_count in support_data.items():
        if len(itemset) == 2:
            item1, item2 = list(itemset)
            item1 = frozenset([item1])
            item2 = frozenset([item2])
            if item1 in support_data and item2 in support_data:
                sup_union = support_count / total_transactions
                prob1 = support_data[item1] / total_transactions
                prob2 = support_data[item2] / total_transactions
                lift = sup_union / (prob1 * prob2)
                
                if 0.8 <= lift <= 1.2:
                    coincidences.append({
                        'items': list(itemset),
                        'lift': lift,
                        'support': support_count,
                        'expected_support': int(prob1 * prob2 * total_transactions)
                    })
    
    coincidences.sort(key=lambda x: abs(1.0 - x['lift']))
    print(f"\n[EXTRA ADVANCED] Frequent but 'Irrelevant' Pairs (Spurious Patterns)")
    print("      (These items appear together simply because they are popular, not correlated)")
    
    if not coincidences:
        print("      * No spurious patterns (Lift ~ 1.0) detected.")
        return
        
    for c in coincidences[:5]:
        items = c['items']
        print(f"      * {items[0]}  <-->  {items[1]}")
        print(f"        Observed: {c['support']} times | Expected (Random): {c['expected_support']} times")
        print(f"        Lift: {c['lift']:.2f} (Close to 1.0 = No Correlation)")