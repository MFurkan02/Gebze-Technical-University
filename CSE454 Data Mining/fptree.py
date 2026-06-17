import csv
import sys
import os
import time
from collections import defaultdict
import utils  

# Recursion Limit Patch
sys.setrecursionlimit(20000)

# ==========================================
# SETTINGS
# ==========================================
INPUT_FILE = 'online_retail_II.csv'
OUTPUT_FILE = 'results_fptree.txt'
MIN_SUPPORT = 100

# ==========================================
# 1. DATA STRUCTURES
# ==========================================
class FPNode:
    def __init__(self, name, count, parent):
        self.name = name
        self.count = count
        self.parent = parent
        self.children = {}
        self.node_link = None
    def increment(self, amount):
        self.count += amount

def update_header(node_to_test, target_node):
    while node_to_test.node_link is not None:
        node_to_test = node_to_test.node_link
    node_to_test.node_link = target_node

# ==========================================
# 2. TREE CONSTRUCTION (FIXED SORTING)
# ==========================================
def create_tree(dataset, min_sup):
    header_table = defaultdict(int)
    for trans in dataset:
        for item in trans:
            header_table[item] += dataset[trans]
    header_table = {k: v for k, v in header_table.items() if v >= min_sup}
    freq_item_set = set(header_table.keys())
    if len(freq_item_set) == 0: return None, None
    for k in header_table:
        header_table[k] = [header_table[k], None]
    ret_tree = FPNode('Null Set', 1, None)
    for trans, count in dataset.items():
        local_d = {}
        for item in trans:
            if item in freq_item_set:
                local_d[item] = header_table[item][0]
        if len(local_d) > 0:
            ordered_items = [v[0] for v in sorted(local_d.items(), key=lambda p: (p[1], p[0]), reverse=True)]
            update_tree(ordered_items, ret_tree, header_table, count)
    return ret_tree, header_table

def update_tree(items, in_tree, header_table, count):
    if items[0] in in_tree.children:
        in_tree.children[items[0]].increment(count)
    else:
        in_tree.children[items[0]] = FPNode(items[0], count, in_tree)
        if header_table[items[0]][1] is None:
            header_table[items[0]][1] = in_tree.children[items[0]]
        else:
            update_header(header_table[items[0]][1], in_tree.children[items[0]])
    if len(items) > 1:
        update_tree(items[1::], in_tree.children[items[0]], header_table, count)

# ==========================================
# 3. PATTERN MINING (FIXED COUNTING)
# ==========================================
def ascend_tree(leaf_node, prefix_path):
    if leaf_node.parent is not None:
        prefix_path.append(leaf_node.name)
        ascend_tree(leaf_node.parent, prefix_path)

def find_prefix_path(base_pat, tree_node):
    cond_pats = {}
    while tree_node is not None:
        prefix_path = []
        ascend_tree(tree_node, prefix_path)
        if len(prefix_path) > 1:
            path_key = frozenset(prefix_path[1:])
            cond_pats[path_key] = cond_pats.get(path_key, 0) + tree_node.count
        tree_node = tree_node.node_link
    return cond_pats

def mine_tree(in_tree, header_table, min_sup, pre_fix, freq_item_list):
    big_l = [v[0] for v in sorted(header_table.items(), key=lambda p: (p[1][0], p[0]))]
    for base_pat in big_l:
        new_freq_set = pre_fix.copy()
        new_freq_set.add(base_pat)
        count = header_table[base_pat][0]
        freq_item_list.append((new_freq_set, count))
        cond_patt_bases = find_prefix_path(base_pat, header_table[base_pat][1])
        my_cond_tree, my_head = create_tree(cond_patt_bases, min_sup)
        if my_head is not None:
            mine_tree(my_cond_tree, my_head, min_sup, new_freq_set, freq_item_list)

# ==========================================
# 4. HELPER FUNCTIONS
# ==========================================
def load_data(filename):
    if not os.path.exists(filename):
        print(f"ERROR: '{filename}' not found!")
        sys.exit(1)
    print(f"Reading file [{filename}]...")
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

def convert_to_init_set(data_set):
    ret_dict = defaultdict(int)
    for trans in data_set:
        ret_dict[frozenset(trans)] += 1
    return ret_dict

# ==========================================
# 5. MAIN
# ==========================================
if __name__ == "__main__":
    raw_data = load_data(INPUT_FILE)
    
    if raw_data:
        init_set = convert_to_init_set(raw_data)
        total_trans = len(raw_data)
        
        print(f"\n--- STARTING FP-TREE (Min Sup: {MIN_SUPPORT}) ---")
        
        utils.start_memory_profile()
        start_time = time.time()
        
        my_fp_tree, my_header_tab = create_tree(init_set, MIN_SUPPORT)
        freq_items = []
        if my_fp_tree:
            mine_tree(my_fp_tree, my_header_tab, MIN_SUPPORT, set([]), freq_items)
            
        peak_memory = utils.stop_memory_profile()
        end_time = time.time()
        
        total_duration = end_time - start_time
        max_mem_mb = peak_memory / 10**6
        
        print("\n" + "="*40)
        print("PERFORMANCE REPORT (FP-TREE)")
        print("="*40)
        print(f"Total Time         : {total_duration:.4f} seconds")
        print(f"Peak Memory        : {max_mem_mb:.2f} MB")
        print("="*40)
        
        # --- RECOMMENDATION LOGIC ---
        if freq_items:
            print(f"\n[ENGINEERING] Advanced Recommendation System (Lift & Leverage)...")
            support_data = {frozenset(item): count for item, count in freq_items}
            
            # Generate Rules
            rules = utils.generate_advanced_rules(support_data, total_trans, min_conf=0.1, min_lift=1.2)
            
            if not rules:
                print(" -> No strict rules found, loosening criteria (Fallback Mode)...")
                rules = utils.generate_advanced_rules(support_data, total_trans, min_conf=0.01, min_lift=0.8)

            if rules:
                print(f"[RESULT] Found {len(rules)} significant associations.")
                
                try:
                    rules.sort(key=lambda x: x['lift'] * (x.get('support_count', 1)**0.5), reverse=True)
                except:
                    rules.sort(key=lambda x: x['lift'], reverse=True)
                
                best_product = rules[0]['ant']
                
                print(f"\n[AUTO-SELECT] Best product/basket selected for demo: {best_product}")
                utils.print_recommendations(rules, best_product)
                
                utils.detect_coincidental_patterns(support_data, total_trans)

                # --- FILE WRITING BLOCK ---
                print(f"\nWriting detailed report to '{OUTPUT_FILE}'...")
                try:
                    with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
                        f.write("==================================================\n")
                        f.write("         FP-TREE ALGORITHM ANALYSIS REPORT        \n")
                        f.write("==================================================\n\n")
                        
                        f.write(f"Dataset      : {INPUT_FILE}\n")
                        f.write(f"Min Support  : {MIN_SUPPORT}\n")
                        f.write(f"Exec Time    : {total_duration:.4f} sec\n")
                        f.write(f"Peak Memory  : {max_mem_mb:.2f} MB\n\n")

                        # --- Grouping and Sorting Logic ---
                        # 1. Group patterns by their length
                        grouped_items = defaultdict(list)
                        for pattern, count in freq_items:
                            grouped_items[len(pattern)].append((pattern, count))

                        # 2. Get lengths and sort them from largest to smallest
                        sorted_lengths = sorted(grouped_items.keys(), reverse=True)

                        f.write("==================================================\n")
                        f.write("           FOUND FREQUENT ITEMSETS                \n")
                        f.write("      (Sorted by Size: Largest to Smallest)       \n")
                        f.write("==================================================\n")

                        for length in sorted_lengths:
                            f.write(f"\n>>> {length}-ITEMSETS <<<\n")
                            f.write("-" * 30 + "\n")
                            
                            # 3. Sort itemsets within the same length group by their count
                            current_group = sorted(grouped_items[length], key=lambda x: x[1], reverse=True)
                            
                            for p, count in current_group:
                                # Convert frozenset/set to a readable string
                                items_str = ", ".join(list(p))
                                f.write(f"Count: {count:<6} | Items: [{items_str}]\n")

                        # Association Rules at the end
                        f.write("\n\n" + "="*50 + "\n")
                        f.write(" TOP 20 ASSOCIATION RULES (by Lift)\n")
                        f.write("="*50 + "\n")
                        for i, r in enumerate(rules[:20]):
                            f.write(f"{i+1}. {list(r['ant'])} -> {list(r['cons'])}\n")
                            f.write(f"   Lift: {r['lift']:.2f} | Conf: {r['conf']*100:.1f}%\n\n")

                    print(" -> Report successfully saved with grouped itemsets!")
                except Exception as e:
                    print(f" -> ERROR writing file: {e}")