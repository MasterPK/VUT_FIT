//Petr Křehlík (xkrehl04)
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
public class countdown : MonoBehaviour
{
    public TMP_Text countdownTxt;
    public int timeLeft;//Seconds Overall
    // Start is called before the first frame update
    void Start()
    {
        timeLeft = SettingsHandler.maxTimeSelected;
        if(timeLeft!=0)
        {
            countdownTxt.text = timeLeft.ToString() + ":00";
            timeLeft *= 60;
            StartCoroutine("LoseTime");
            Time.timeScale = 1; //Just making sure that the timeScale is right
        }
        else
        {
            TMP_Text tmp = GameObject.Find("timeLeftTxt").GetComponent<TMP_Text>();
            tmp.enabled = false;
            countdownTxt.enabled = false;
        }
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    IEnumerator LoseTime()
    {
        while (true)
        {
            yield return new WaitForSeconds(1);
            timeLeft--;
            if(timeLeft / 60 >= 1)
            {
                int minutes = timeLeft / 60;
                int seconds = timeLeft % 60;
                countdownTxt.text = minutes + ":" + seconds;
            }
            else
            {
                countdownTxt.text = "00:"+timeLeft.ToString();
            }
            
            if (timeLeft == 0)
            {
                Canvas tmp = GameObject.FindGameObjectsWithTag("endGamePanel")[0].GetComponent<Canvas>();
                tmp.enabled = true;
                TMP_Text tmp2 = GameObject.Find("endGameMsg").GetComponent<TMP_Text>();
                tmp2.text = setLanguage.LMan.getString("looser");
                break;
            }

        }
    }
}
