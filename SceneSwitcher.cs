using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneSwitcher : MonoBehaviour
{
    private string sceneA = "Basic";
    private string sceneB = "Nuclear";
    private float switchInterval = 75.0f; 

    // Use this for initialization
    void Start()
    {
        DontDestroyOnLoad(gameObject);
        // Start the coroutine to switch scenes
        StartCoroutine(SwitchSceneAfterTime(sceneA, sceneB, switchInterval));
    }

    IEnumerator SwitchSceneAfterTime(string sceneA, string sceneB, float interval)
    {
        // Wait for the specified interval
        yield return new WaitForSeconds(interval);

        // Check the current scene and switch to the other one
        string currentScene = SceneManager.GetActiveScene().name;
        if(currentScene == sceneA)
        {
            SceneManager.LoadScene(sceneB);
        }
        else
        {
            SceneManager.LoadScene(sceneA);
        }

        // Restart the coroutine to switch back after time
        StartCoroutine(SwitchSceneAfterTime(sceneA, sceneB, interval));
    }
}
