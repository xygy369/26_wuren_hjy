import torch
import torch.nn as nn
import torch.optim as optim  #优化器 更新参数
from model import ConeCNN  #导入cnn网络类
from dataset import get_dataloaders #导入加载数据


DATA_DIR = '/home/enovogyxy/dataset'  #固定文件夹位置
BATCH_SIZE = 32  #每次32张图
EPOCHS = 20  #训练20轮
LEARNING_RATE = 0.001  #学习率  控制步长


device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
print(f"使用设备: {device}")  #是否能用GPU

train_loader, test1_loader, test2_loader = get_dataloaders(DATA_DIR, BATCH_SIZE)
print(f"训练集: {len(train_loader.dataset)} 张图片")  #打包数据

model = ConeCNN(num_classes=3).to(device)  #将cnn模型放在GPU上运行
print("\n网络结构:")
print(model) #显示网络结构

criterion = nn.CrossEntropyLoss() #损失函数 衡量猜错程度
optimizer = optim.Adam(model.parameters(), lr=LEARNING_RATE)  #优化器 调整的参数和学习率

print("\n开始训练...")
for epoch in range(EPOCHS):
    model.train() #训练模式
    running_loss = 0.0
    correct = 0
    total = 0
    for i, (images, labels) in enumerate(train_loader):
        images, labels = images.to(device), labels.to(device) #放在GPU训练
        outputs = model(images)  #预测结果
        loss = criterion(outputs, labels) #损失率
        optimizer.zero_grad() #梯度清零
        loss.backward()  #反向传播 重新调整
        optimizer.step()  #更新参数
        running_loss += loss.item()  #以下为更新数据
        _, predicted = torch.max(outputs, 1)
        total += labels.size(0)
        correct += (predicted == labels).sum().item()
        if (i + 1) % (len(train_loader) // 2 + 1) == 0:
            print(f"  Epoch [{epoch+1}/{EPOCHS}], Step [{i+1}/{len(train_loader)}], Loss: {loss.item():.4f}")
    avg_loss = running_loss / len(train_loader)
    accuracy = 100 * correct / total
    print(f"  轮次 [{epoch+1}/{EPOCHS}] 完成, 平均损失: {avg_loss:.4f}, 准确率: {accuracy:.2f}%\n")

torch.save(model.state_dict(), 'cone_model.pth')
print("训练完成，模型已保存为 cone_model.pth")
