using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class ChangeColor : MonoBehaviour
{

    public void OnButtonClicked(Button clickedButton)
    {

        GameObject.Find("Button0").GetComponent<Button>().interactable = true;
        GameObject.Find("Button1").GetComponent<Button>().interactable = true;
        GameObject.Find("Button2").GetComponent<Button>().interactable = true;
        GameObject.Find("Button3").GetComponent<Button>().interactable = true;
        GameObject.Find("Button5").GetComponent<Button>().interactable = true;
        GameObject.Find("Button6").GetComponent<Button>().interactable = true;
        GameObject.Find("Button7").GetComponent<Button>().interactable = true;
        GameObject.Find("Button8").GetComponent<Button>().interactable = true;

        clickedButton.interactable = false;

        GameObject.Find("PageText").GetComponent<TMP_Text>().fontSize = 35;
        GameObject.Find("PageText").GetComponent<TMP_Text>().alignment = TMPro.TextAlignmentOptions.TopLeft;

        switch (clickedButton.name)
        {
            case "Button0":
                GameObject.Find("PageText").GetComponent<TMP_Text>().alignment = TMPro.TextAlignmentOptions.Center;
                GameObject.Find("PageText").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("welcomeMsg");
                GameObject.Find("Headline").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualBtn1");
                break;
            case "Button1":
                GameObject.Find("PageText").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualTxt1");
                GameObject.Find("Headline").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualBtn2");
                break;
            case "Button2":
                GameObject.Find("PageText").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualTxt2");
                GameObject.Find("Headline").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualBtn3");
                break;
            case "Button3":
                GameObject.Find("PageText").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualTxt3");
                GameObject.Find("Headline").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualBtn4");
                break;
            case "Button5":
                GameObject.Find("PageText").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualTxt4");
                GameObject.Find("Headline").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualBtn6");
                break;
            case "Button6":
                GameObject.Find("PageText").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualTxt5");
                GameObject.Find("Headline").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualBtn7");
                break;
            case "Button7":
                GameObject.Find("PageText").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualTxt6");
                GameObject.Find("Headline").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualBtn8");
                break;
            case "Button8":
                GameObject.Find("PageText").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualTxt7");
                GameObject.Find("Headline").GetComponent<TMP_Text>().text = setLanguage.LMan.getString("manualBtn9");
                break;
        }

    }
}
