import os   #查询文件路径
from torch.utils.data import Dataset, DataLoader   #获取文件
from torchvision import transforms   #转换文件格式
from PIL import Image


class ConeDataset(Dataset):  #锥桶数据集的类
    def __init__(self, root_dir, transform=None):   #继承dataset模板
        self.transform = transform  #图片处理方法
        self.images = []   #作为列表存储每个dataset中的图片路径
        self.labels = []   #存储每个图片的标签  红黄蓝
        self.classes = ['blue', 'red', 'yellow']
        class_to_idx = {'blue': 0, 'red': 1, 'yellow': 2}  #字典转换
        for class_name in self.classes:   #遍历dataset文件夹
            class_dir = os.path.join(root_dir, class_name)   #拼装路径 /dataset/train  +/blue等
            if not os.path.exists(class_dir):
                continue  #跳过
            for img_name in os.listdir(class_dir):  #遍历每个文件夹下的图片
                if img_name.endswith(('.jpg', '.png', '.jpeg')):  #通过后缀判断要处理的文件
                    self.images.append(os.path.join(class_dir, img_name))  #存路径
                    self.labels.append(class_to_idx[class_name])  #存标签

    def __len__(self):
        return len(self.images)  #获取图片数量

    def __getitem__(self, idx):  #获取单个图片
        img_path = self.images[idx]
        label = self.labels[idx]
        image = Image.open(img_path).convert('RGB') #转换成RGB格式
        if self.transform:
            image = self.transform(image)
        return image, label   #返回


def get_transforms():    #处理图片
    train_transform = transforms.Compose([
        transforms.Resize((64, 64)), #统一转换为64*64分辨率
        transforms.RandomHorizontalFlip(),  #随即水平翻转   ai说不容易过拟合，提高多样性
        transforms.RandomRotation(10),   #随机选注+-10度   ai说同样防止过拟合
        transforms.ToTensor(),   #****将图片转成张量（从0~255到0.0~1.0）
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])  #归一化，让数据分布合理 ai说这个是经验值，表示三个平均像素值 0到1的   还有平均标准差
    ])
    test_transform = transforms.Compose([
        transforms.Resize((64, 64)),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]) #测试集不反转增强
    return train_transform, test_transform


def get_dataloaders(data_dir, batch_size=32):  #数据文件夹路径   一次取32张图
    train_transform, test_transform = get_transforms()   #定义好的函数
    train_dataset = ConeDataset(os.path.join(data_dir, 'train'), train_transform)
    test1_dataset = ConeDataset(os.path.join(data_dir, 'test1'), test_transform)
    test2_dataset = ConeDataset(os.path.join(data_dir, 'test2'), test_transform)
    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)  #重点 True表示打乱顺序
    test1_loader = DataLoader(test1_dataset, batch_size=batch_size, shuffle=False)
    test2_loader = DataLoader(test2_dataset, batch_size=batch_size, shuffle=False)
    return train_loader, test1_loader, test2_loader   #返回预处理好的数据加载器，等待测试
