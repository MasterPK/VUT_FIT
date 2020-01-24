//Petr Křehlík (xkrehl04)
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class setLanguageGame : MonoBehaviour
{
    public TMP_Text resetBtnTxt;
    public TMP_Text newGameBtnTxt;
    public TMP_Text timeLeftTxt;
    public TMP_Text resetTxt;
    public TMP_Text newGameTxt;
    public Sprite blackEng;
    public Sprite whiteEng;
    public Image up;
    public Image down;
    // Start is called before the first frame update
    void Start()
    {
        resetBtnTxt.text= setLanguage.LMan.getString("resetBtnTxt");
        newGameBtnTxt.text = setLanguage.LMan.getString("newGameBtnTxt");
        resetTxt.text = setLanguage.LMan.getString("resetBtnTxt");
        newGameTxt.text = setLanguage.LMan.getString("newGameBtnTxt");
        timeLeftTxt.text = setLanguage.LMan.getString("timeLeftTxt");
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
