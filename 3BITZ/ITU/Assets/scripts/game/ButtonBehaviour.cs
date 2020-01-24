using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Threading;

public class ButtonBehaviour : MonoBehaviour 
{

    void Start()
    {
     
    }
    void Update()
    {
    }


    public void RestartGame()
    {
        Canvas tmp = GameObject.FindGameObjectsWithTag("endGamePanel")[0].GetComponent<Canvas>();
        tmp.enabled = false;
        SceneManager.LoadScene(SceneManager.GetActiveScene().name);
        
    }

    public void NewGame()
    {
        SceneManager.LoadScene(0);
    }
}
