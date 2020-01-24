//Petr Křehlík (xkrehl04)
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
public class remainingCountEnemy : MonoBehaviour
{
    public PieceManager pieceManager;
    public TMP_Text enemyCount;
    public Image downImage;
    public Image upImage;
    // Start is called before the first frame update
    void Start()
    {

        
    }

    // Update is called once per frame
    void Update()
    {

    enemyCount.text = pieceManager.BlackLeft.ToString();
    }
}
