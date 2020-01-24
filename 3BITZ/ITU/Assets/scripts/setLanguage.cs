//Petr Křehlík (xkrehl04)
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class setLanguage : MonoBehaviour
{
    // Start is called before the first frame update
    public static Lang LMan;
    public static string currentLang = null;
    private string previousLang;
    public TextMeshProUGUI color_select_txt;
    public TextMeshProUGUI difficulty_select_txt;
    public TextMeshProUGUI max_time_select_txt;
    public Button button;
    public Button button2;
    public TextMeshProUGUI playButton;
    public TMP_Dropdown dropdown;
    public TextMeshProUGUI easy_txt;
    public TextMeshProUGUI hard_txt;
    public TextMeshProUGUI tutorial;
    public Slider difficultySlider;
    public TextMeshProUGUI difficultySliderText;
    public TextMeshProUGUI multiplayerBtnTxt;
    void Start()
    {
        if(currentLang==null)
        {
            //This checks if your computer's operating system is in the Czech/Slovak language
            if (Application.systemLanguage == SystemLanguage.Czech || Application.systemLanguage == SystemLanguage.Slovak)
            {
                currentLang = "Czech";
                previousLang = "Czech";
            }
            //Otherwise, if the system is something else, set English
            else
            {
                currentLang = "English";
                previousLang = "English";
            }
            LMan = new Lang(Path.Combine(Application.dataPath, "./language.xml"), currentLang, false);
        }
        
        SetTexts();

        try
        {
            button.onClick.AddListener(TaskOnClick);
            button2.onClick.AddListener(TaskOnClick2);
        }
        catch
        { }
    }

    private void TaskOnClick()
    {
        SetLanguage("Czech");
    }

    private void TaskOnClick2()
    {
        SetLanguage("English");
    }

    private void SetTexts()
    {
        color_select_txt.text = LMan.getString("color_select_txt");
        difficulty_select_txt.text = LMan.getString("difficulty_select_txt");
        max_time_select_txt.text = LMan.getString("max_time_select_txt");
        easy_txt.text = LMan.getString("easy");
        hard_txt.text = LMan.getString("hard");
        playButton.text = LMan.getString("play");
        dropdown.options = new List<TMP_Dropdown.OptionData>{ };
        TMP_Dropdown.OptionData tmp = new TMP_Dropdown.OptionData();
        tmp.text = LMan.getString("none");
        dropdown.options.Add(tmp);
        tmp = new TMP_Dropdown.OptionData();
        tmp.text = LMan.getString("3_min");
        dropdown.options.Add(tmp);
        tmp = new TMP_Dropdown.OptionData();
        tmp.text = LMan.getString("5_min");
        dropdown.options.Add(tmp);
        tmp = new TMP_Dropdown.OptionData();
        tmp.text = LMan.getString("10_min");
        dropdown.options.Add(tmp);
        dropdown.value = 1;
        dropdown.value = 0;
        tutorial.text= LMan.getString("tutorial");
        multiplayerBtnTxt.text= LMan.getString("multiplayerBtnTxt"); 
        if (difficultySlider.value == 0)
        {
            difficultySliderText.text = LMan.getString("easy");
        }
        if (difficultySlider.value == 1)
        {
            difficultySliderText.text = LMan.getString("medium");

        }
        if (difficultySlider.value == 2)
        {
            difficultySliderText.text = LMan.getString("hard");
        }
    }

    public void SetLanguage(string newLang)
    {
        currentLang = newLang;
    }

    // Update is called once per frame
    void Update()
    {
        if(previousLang!=currentLang)
        {
            previousLang = currentLang;
            LMan.setLanguage("./language.xml", currentLang);
            SetTexts();
        }
        
    }
}
