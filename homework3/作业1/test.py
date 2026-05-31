import torch
from model import ConeCNN
from dataset import get_dataloaders


DATA_DIR = '/home/enovogyxy/dataset'

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
print(f"使用设备: {device}")

_, test1_loader, test2_loader = get_dataloaders(DATA_DIR, batch_size=32) #返回两个测试集的加载器

model = ConeCNN(num_classes=3).to(device)   #空网络
model.load_state_dict(torch.load('cone_model.pth')) #加载train.py的参数
model.eval()  #测试模式

classes = ['blue', 'red', 'yellow']


def evaluate(model, loader, device, dataset_name): #测试函数
    correct = 0
    total = 0
    class_correct = [0, 0, 0]  #答对总数
    class_total = [0, 0, 0]  #每个类别的总数
    with torch.no_grad():  #关闭梯度计算   测试时不用  不需要更新参数
        for images, labels in loader:
            images, labels = images.to(device), labels.to(device)
            outputs = model(images)  #前向传播
            _, predicted = torch.max(outputs, 1) #取最大值位置
            total += labels.size(0) #数量
            correct += (predicted == labels).sum().item() #答对数量
            for i in range(labels.size(0)):
                label = labels[i].item()
                class_total[label] += 1
                if predicted[i] == labels[i]:
                    class_correct[label] += 1
    print(f"\n{'='*50}")
    print(f"【{dataset_name}】测试结果")
    print(f"{'='*50}")
    print(f"整体正确率: {correct}/{total} = {100*correct/total:.2f}%\n")
    print("各类别正确率:")
    for i, cls in enumerate(classes):
        if class_total[i] > 0:
            acc = 100 * class_correct[i] / class_total[i]  #正确率
            print(f"  {cls:>6}: {class_correct[i]:>3}/{class_total[i]:<3} = {acc:.2f}%")


evaluate(model, test1_loader, device, "测试集1 (test1)")
evaluate(model, test2_loader, device, "测试集2 (test2)")
print("\n测试完成！")
