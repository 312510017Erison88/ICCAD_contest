import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import matplotlib.patches as patches

# read blocks.csv
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

# read path.csv
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

### erison's code
# read path.csv
# net_paths = []
# with open('path.csv', 'r') as file:
#     for line in file:
#         if line.startswith("Net ID:"):
#             net_id = line.strip().split(' ')[-1]
#         else:
#             vertices = []
#             points = line.strip().split(') (')
#             for point in points:
#                 if point:
#                     x, y = point.strip('()').split(',')
#                     try:
#                         vertices.append((int(x), int(y)))
#                     except ValueError:
#                         print("Error converting point:", point)
#             net_paths.append(vertices)
#####


# 創建一個繪圖
fig, ax = plt.subplots(dpi=300)     # 2000 dpi (suggest)

# 繪製每個區塊
for block in blocks:
    vertices = block['vertices']
    polygon = patches.Polygon(vertices, closed=True, fill=None, edgecolor='r', linewidth=0.5)
    ax.add_patch(polygon)
    # 標示區塊名稱
    centroid_x = sum([v[0] for v in vertices]) / len(vertices)
    centroid_y = sum([v[1] for v in vertices]) / len(vertices)
    ax.text(centroid_x, centroid_y, block['block_name'], ha='center', va='center', fontsize=3)

### erison's code
# 繪製每個網絡路徑
# for path in net_paths:
#     xs, ys = zip(*path)
#     ax.plot(xs, ys, color='blue', linewidth=1.5)
#####

# draw the path of each net
for net in nets:
    for path in net['paths']:
        x_vals, y_vals = zip(*path)
        line = mlines.Line2D(x_vals, y_vals, linewidth=1, color='b')
        ax.add_line(line)


# 設置圖形範圍
ax.set_xlim(0, 12440136)
ax.set_ylim(0, 10368720)

# ax.set_xlim(0, 10000)
# ax.set_ylim(0, 10000)

ax.set_aspect('equal')

# 顯示圖形
plt.show()