using UnityEngine;
using OscJack;
using UnityEngine.UIElements;

public class ServerTest : MonoBehaviour
{
    public UIDocument OSCDATA; // 通过Inspector指定
    private OscServer _server; // 使用下划线前缀以匹配您的私有变量声明
    private Label OSCLabel; // UI Label用于显示数据
    Renderer ren;
    Color currentColor;
    bool colorUpdated = false;

    void Start()
    {
        ren = GetComponent<Renderer>();
        currentColor = ren.material.color; // 从材质中获取当前颜色

        // 初始化OSC服务器
        _server = new OscServer(9005); // 正确使用声明的变量名

        // 为/channel1设置回调
        _server.MessageDispatcher.AddCallback("/chan1", (string address, OscDataHandle data) => {
            if (data.GetElementCount() > 0)
            {
                float value = data.GetElementAsFloat(0);
                Debug.Log($"Received {value} in ServerTest");
                UpdateLabel(value); // 在这里调用更新Label的方法
            }
            else
            {
                Debug.Log("OSC message received but contains no elements to process.");
            }
        });

        // 为/r, /g, /b设置回调
        _server.MessageDispatcher.AddCallback("/r", (string address, OscDataHandle data) => {
            if (data.GetElementCount() > 0)
            {
                currentColor.r = data.GetElementAsFloat(0);
                colorUpdated = true; // 标记颜色已更新
            }
        });

        _server.MessageDispatcher.AddCallback("/g", (string address, OscDataHandle data) => {
            if (data.GetElementCount() > 0)
            {
                currentColor.g = data.GetElementAsFloat(0);
                colorUpdated = true; // 标记颜色已更新
            }
        });

        _server.MessageDispatcher.AddCallback("/b", (string address, OscDataHandle data) => {
            if (data.GetElementCount() > 0)
            {
                currentColor.b = data.GetElementAsFloat(0);
                colorUpdated = true; // 标记颜色已更新
            }
        });

        // 设置UI
        var rootVisualElement = OSCDATA.rootVisualElement;
        OSCLabel = rootVisualElement.Q<Label>("OSCLabel");
        if (OSCLabel == null)
        {
            Debug.LogError("OSC Label not found!");
        }
        else
        {
            Debug.Log("OSC Label found successfully.");
        }
    }

    void Update()
    {
        // 如果颜色已更新，则在 Update 中应用新颜色
        if (colorUpdated)
        {
            ren.material.color = currentColor; // 应用新颜色
            colorUpdated = false; // 重置颜色更新标记
        }
    }

    // 定义一个方法来更新Label
    void UpdateLabel(float value)
    {
        if (OSCLabel != null)
        {
            OSCLabel.text = $"Distance: {value}"; // 设置Label文本
        }
    }

    void OnDestroy()
    {
        _server?.Dispose(); // 正确地释放服务器资源
    }
}
