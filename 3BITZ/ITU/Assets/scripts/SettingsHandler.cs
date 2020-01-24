//Petr Křehlík (xkrehl04)
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.UI;
using TMPro;
using UnityEngine.SceneManagement;

public class SettingsHandler : MonoBehaviour
{
    public Slider difficultySlider;
    public TextMeshProUGUI difficultySliderText;
    private setLanguage setLanguage;
    public Image whiteImgFrame;
    public Image blackImgFrame;

    public static bool colorSelected;
    public static int difficultySelected;
    public static int maxTimeSelected;
    public static bool multiplayer;

    void Start()
    {
        try
        {
            difficultySlider.onValueChanged.AddListener(difficultySettingsChanged);
        }
        catch { };
        
    }
    public void SetActiveWhite()
    {
        whiteImgFrame.enabled = true;
        blackImgFrame.enabled = false;
        colorSelected = false;
    }

    public void SetActiveBlack()
    {
        whiteImgFrame.enabled = false;
        blackImgFrame.enabled = true;
        colorSelected = true;
    }

    void Update()
    {
       

    }
    public void difficultySettingsChanged(float value)
    {
        difficultySelected =(int) value;
        if (value == 0)
        {
            difficultySliderText.text = setLanguage.LMan.getString("easy");
        }
        if (value == 1)
        {
            difficultySliderText.text = setLanguage.LMan.getString("medium");

        }
        if (value == 2)
        {
            difficultySliderText.text = setLanguage.LMan.getString("hard");
        }
    }

    public void maxTimeSettingsChanged(TMP_Dropdown dropdown)
    {
        
        if (dropdown.value == 0)
        {
            maxTimeSelected = 0;
        }
        else if (dropdown.value == 1)
        {
            maxTimeSelected = 3;

        }
        else if (dropdown.value == 2)
        {
            maxTimeSelected = 5;
        }
        else if (dropdown.value == 3)
        {
            maxTimeSelected = 10;
        }
        else
        {
            maxTimeSelected = -1;
        }

    }

    public void PlayGame()
    {
        multiplayer = false;
        SceneManager.LoadScene(1);
    }

    public void PlayLocalGame()
    {
        multiplayer = true;
        SceneManager.LoadScene(1);
    }

    public void OpenTutorial()
    {
        SceneManager.LoadScene(2);
    }

    public void CloseTutorial()
    {
        SceneManager.LoadScene(0);
    }


}
