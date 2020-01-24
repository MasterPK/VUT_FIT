//Petr Křehlík (xkrehl04)
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class remainingCount : MonoBehaviour
{
    public PieceManager pieceManager;
    public TMP_Text playerCount;
    public Image downImage;
    public Image upImage;
    public Sprite black;
    public Sprite white;
    public Sprite whiteEng;
    public Sprite blackEng;
    // Start is called before the first frame update
    void Start()
    {
        if (setLanguage.currentLang == "Czech" || setLanguage.currentLang == "Slovak")
        {
            if (!SettingsHandler.colorSelected)
            {
                upImage.sprite = black;
                downImage.sprite = white;
            }
            else
            {
                upImage.sprite = white;
                downImage.sprite = black;
            }
        }
        else
        {
            if (!SettingsHandler.colorSelected)
            {
                upImage.sprite = blackEng;
                downImage.sprite = whiteEng;
            }
            else
            {
                upImage.sprite = whiteEng;
                downImage.sprite = blackEng;
            }
        }

    }

    // Update is called once per frame
    void Update()
    {
        playerCount.text=pieceManager.WhiteLeft.ToString();
    }
}
