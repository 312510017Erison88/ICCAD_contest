import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import matplotlib.patches as patches

# 讀取CSV資料
blocks = []
with open('blocks_feedthrough.csv', 'r') as file:
    for line in file:
        parts = line.strip().split(',')
        block_name = parts[0]
        vertices = []
        for vertex in parts[1:]:
            if vertex:
                x, y = vertex.split(';')
                try:
                    vertices.append((int(x), int(y)))
                except ValueError:
                    print("Error converting vertex:", vertex)
        blocks.append({'block_name': block_name, 'vertices': vertices})
        
        
nonfeedblocks = []
with open('blocks_nonfeedthrough.csv', 'r') as file:
    for line in file:
        parts = line.strip().split(',')
        block_name = parts[0]
        vertices = []
        for vertex in parts[1:]:
            if vertex:
                x, y = vertex.split(';')
                try:
                    vertices.append((int(x), int(y)))
                except ValueError:
                    print("Error converting vertex:", vertex)
        nonfeedblocks.append({'block_name': block_name, 'vertices': vertices})
        
        
        
        
        
# read regions.csv
regions = []
with open('regions.csv', 'r') as file:
    for line in file:
        parts = line.strip().split(',')
        region_name = parts[0]
        vertices = []
        for vertex in parts[1:]:
            if vertex:
                x, y = vertex.split(';')
                try:
                    vertices.append((int(x), int(y)))
                except ValueError:
                    print("Error converting vertex:", vertex)
        regions.append({'region_name': region_name, 'vertices': vertices})


        
nets = []
with open('path.csv', 'r') as file:
    current_net = None
    for line in file:
        line = line.strip()
        if line.startswith('Net ID:'):
            if current_net:
                nets.append(current_net)
            current_net = {'net_id': line.split(':')[1].strip(), 'paths': []}
        elif line:
            points = line.split(',')
            path = []
            for point in points:
                try:
                    x, y = point.split(';')
                    path.append((int(x), int(y)))
                except ValueError:
                    print(f"Issue with point: {point}. Skipping.")
            if path:
                current_net['paths'].append(path)
    if current_net:  # append the last net if exists
        nets.append(current_net)






# 創建一個繪圖
fig, ax = plt.subplots(dpi=2000)   ## 2000dpi for suggestion

## 繪製每個區塊
for block in blocks:
    vertices = block['vertices']
    polygon = patches.Polygon(vertices, closed=True, fill=None, edgecolor='b', linewidth=0.01)
    ax.add_patch(polygon)
    # 標示區塊名稱
    # centroid_x = sum([v[0] for v in vertices]) / len(vertices)
    # centroid_y = sum([v[1] for v in vertices]) / len(vertices)
    # ax.text(centroid_x, centroid_y, block['block_name'], ha='center', va='center', fontsize=2)

for nonfeedblock in nonfeedblocks:
    vertices = nonfeedblock['vertices']
    polygon = patches.Polygon(vertices, closed=True, fill=None, edgecolor='k', linewidth=0.01)
    ax.add_patch(polygon)
    # 標示區塊名稱
    # centroid_x = sum([v[0] for v in vertices]) / len(vertices)
    # centroid_y = sum([v[1] for v in vertices]) / len(vertices)
    # ax.text(centroid_x, centroid_y, nonfeedblock['block_name'], ha='center', va='center', fontsize=2)


## 繪製每個區塊
for region in regions:
    vertices = region['vertices']
    polygon = patches.Polygon(vertices, closed=True, fill=None, edgecolor='g', linewidth=0.01)
    ax.add_patch(polygon)
    # # 標示區塊名稱
    # centroid_x = sum([v[0] for v in vertices]) / len(vertices)
    # centroid_y = sum([v[1] for v in vertices]) / len(vertices)
    # ax.text(centroid_x, centroid_y, region['region_name'], ha='center', va='center', fontsize=2)







# paths = [
#     [(0, 0), (100, 200), (200, 100), (300, 300)],  # 第一條路徑
#     [(50, 50), (150, 250), (250, 150), (350, 350)]  # 第二條路徑
# ]


# 繪製每個net的路徑
for net in nets:
    for path in net['paths']:
        x_vals, y_vals = zip(*path)
        line = mlines.Line2D(x_vals, y_vals, linewidth=0.05, color='r')
        ax.add_line(line)








# 調整圖的邊框粗細
for spine in ax.spines.values():
    spine.set_linewidth(0.5)  # 設置邊框線條粗細為1
    spine.set_color('gray')  # 設置邊框顏色為灰色

# 調整刻度顯示的粗細和大小
ax.tick_params(axis='both', which='major', labelsize=4, width=0.5, length=2)  # 主刻度
# ax.tick_params(axis='both', which='minor', labelsize=2, width=0.3, length=1)  # 副刻度

# 調整軸標籤的字體大小
ax.xaxis.get_offset_text().set_fontsize(5)
ax.yaxis.get_offset_text().set_fontsize(5)

# 設置圖形範圍
ax.set_xlim(0, 12440136)
ax.set_ylim(0, 10368720)

# ax.set_xlim(0, 10000)
# ax.set_ylim(0, 10000)

ax.set_aspect('equal')

# 顯示圖形
plt.show()
