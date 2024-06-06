import matplotlib.pyplot as plt
import matplotlib.patches as patches

# 讀取CSV資料
blocks = []
with open('blocks.csv', 'r') as file:
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

# 創建一個繪圖
fig, ax = plt.subplots(dpi=300)

# 繪製每個區塊
for block in blocks:
    vertices = block['vertices']
    polygon = patches.Polygon(vertices, closed=True, fill=None, edgecolor='r', linewidth=0.5)
    ax.add_patch(polygon)
    # 標示區塊名稱
    centroid_x = sum([v[0] for v in vertices]) / len(vertices)
    centroid_y = sum([v[1] for v in vertices]) / len(vertices)
    ax.text(centroid_x, centroid_y, block['block_name'], ha='center', va='center', fontsize=3)

# 設置圖形範圍
ax.set_xlim(0, 12440136)
ax.set_ylim(0, 10368720)

# ax.set_xlim(0, 15440136)
# ax.set_ylim(0, 15368720)

ax.set_aspect('equal')

# 顯示圖形
plt.show()