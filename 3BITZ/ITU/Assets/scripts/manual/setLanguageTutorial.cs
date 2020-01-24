using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class setLanguageTutorial : MonoBehaviour
{
    // Start is called before the first frame update
    public TMP_Text button1Txt;
    public Text button2Txt;
    public Text button3Txt;
    public Text button4Txt;
    public TMP_Text button5Txt;
    public TMP_Text button6Txt;
    public TMP_Text button7Txt;
    public Text button8Txt;
    public Text button9Txt;
    public TMP_Text button10Txt;
    public TMP_Text title;
    public TMP_Text welcome;

    void Start()
    {
        button1Txt.text = setLanguage.LMan.getString("manualBtn1");
        button2Txt.text = setLanguage.LMan.getString("manualBtn2");
        button3Txt.text = setLanguage.LMan.getString("manualBtn3");
        button4Txt.text = setLanguage.LMan.getString("manualBtn4");
        button5Txt.text = setLanguage.LMan.getString("manualBtn5");
        button6Txt.text = setLanguage.LMan.getString("manualBtn6");
        button7Txt.text = setLanguage.LMan.getString("manualBtn7");
        button8Txt.text = setLanguage.LMan.getString("manualBtn8");
        button9Txt.text = setLanguage.LMan.getString("manualBtn9");
        button10Txt.text = setLanguage.LMan.getString("manualBtn10");
        welcome.text = setLanguage.LMan.getString("welcomeMsg");
        title.text = setLanguage.LMan.getString("tutorial");
    }

    // Update is called once per frame
    void Update()
    {

    }
}
