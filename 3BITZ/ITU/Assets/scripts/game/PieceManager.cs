using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
public class PieceManager : MonoBehaviour
{

    public bool mIsStillPiece = true;
    public GameObject mPiecePrefab;

    public List<BasePiece> mWhitePieces = null;
    public List<BasePiece> mBlackPieces = null;

    public int WhiteLeft = 12;
    public int BlackLeft = 12;

    public bool blackTurn = false;
    public bool endGame = false;
    private List<BasePiece> mPromotedPieces = new List<BasePiece>();

    private string[] mPieceOrder = new string[24]
    {
        "P", "", "P", "", "P", "", "P", "",
        "", "P", "", "P", "", "P", "", "P",
        "P", "", "P", "", "P", "", "P", ""
    };

    private Dictionary<string, Type> mPieceLibrary = new Dictionary<string, Type>()
    {
        {"P",  typeof(Pawn)},
        {"Q",  typeof(Queen)}
    };

    public void Setup(Board board)
    {
        if(SettingsHandler.colorSelected == false)
        {
            mWhitePieces = CreatePieces(Color.white, new Color32(255, 255, 255, 255), board);
            mBlackPieces = CreatePieces(Color.black, new Color32(0, 0, 0, 255), board);
        }
        else
        {
            mWhitePieces = CreatePieces(Color.white, new Color32(0, 0, 0, 255), board);
            mBlackPieces = CreatePieces(Color.black, new Color32(255, 255, 255, 255), board);
        }

        PlaceWhitePieces(2, 1, 0, mWhitePieces, board);
        PlaceBlackPieces(5,6,7, mBlackPieces, board);

        SwitchSides(Color.black);
    }

    private List<BasePiece> CreatePieces(Color teamColor, Color32 spriteColor, Board board)
    {
        List<BasePiece> newPieces = new List<BasePiece>();

        for (int i = 0; i < mPieceOrder.Length; i++)
        {
            string key = mPieceOrder[i];
            if (key == "")
            {
                continue;
            }




            Type pieceType = mPieceLibrary[key];

            BasePiece newPiece = CreatePiece(pieceType);
            newPieces.Add(newPiece);

            newPiece.Setup(teamColor, spriteColor, this);
        }

        return newPieces;
    }

    private BasePiece CreatePiece(Type pieceType)
    {

        GameObject newPieceObject = Instantiate(mPiecePrefab);
        newPieceObject.transform.SetParent(transform);

        newPieceObject.transform.localScale = new Vector3(1, 1, 1);
        newPieceObject.transform.localRotation = Quaternion.identity;

        BasePiece newPiece = (BasePiece)newPieceObject.AddComponent(pieceType);
        return newPiece;
    }

    private void PlaceWhitePieces(int pawnRow, int pawnRow1, int pawnRow2, List<BasePiece> pieces, Board board)
    {
            pieces[0].Place(board.mAllCells[0, pawnRow]);
            pieces[1].Place(board.mAllCells[2, pawnRow]);
            pieces[2].Place(board.mAllCells[4, pawnRow]);
            pieces[3].Place(board.mAllCells[6, pawnRow]);
            pieces[4].Place(board.mAllCells[1, pawnRow1]);
            pieces[5].Place(board.mAllCells[3, pawnRow1]);
            pieces[6].Place(board.mAllCells[5, pawnRow1]);
            pieces[7].Place(board.mAllCells[7, pawnRow1]);
            pieces[8].Place(board.mAllCells[0, pawnRow2]);
            pieces[9].Place(board.mAllCells[2, pawnRow2]);
            pieces[10].Place(board.mAllCells[4, pawnRow2]);
            pieces[11].Place(board.mAllCells[6, pawnRow2]);
    }

    private void PlaceBlackPieces(int pawnRow, int pawnRow1, int pawnRow2, List<BasePiece> pieces, Board board)
    {
        pieces[0].Place(board.mAllCells[1, pawnRow]);
        pieces[1].Place(board.mAllCells[3, pawnRow]);
        pieces[2].Place(board.mAllCells[5, pawnRow]);
        pieces[3].Place(board.mAllCells[7, pawnRow]);
        pieces[4].Place(board.mAllCells[0, pawnRow1]);
        pieces[5].Place(board.mAllCells[2, pawnRow1]);
        pieces[6].Place(board.mAllCells[4, pawnRow1]);
        pieces[7].Place(board.mAllCells[6, pawnRow1]);
        pieces[8].Place(board.mAllCells[1, pawnRow2]);
        pieces[9].Place(board.mAllCells[3, pawnRow2]);
        pieces[10].Place(board.mAllCells[5, pawnRow2]);
        pieces[11].Place(board.mAllCells[7, pawnRow2]);
    }


    public void Deadlock()
    {
        endGame = true;
        Canvas tmp = GameObject.FindGameObjectsWithTag("endGamePanel")[0].GetComponent<Canvas>();
        tmp.enabled = true;
        TMP_Text tmp2 = GameObject.Find("endGameMsg").GetComponent<TMP_Text>();
        if (BlackLeft == 0)
        {
            tmp2.text = setLanguage.LMan.getString("congrats");
        }
        else
        {
            tmp2.text = setLanguage.LMan.getString("looser");
        }
    }
    private void SetInteractive(List<BasePiece> allPieces, bool value)
    {
        foreach (BasePiece piece in allPieces)
            piece.enabled = value;
    }

    public void SwitchSides(Color color)
    {
        if(WhiteLeft == 0 || BlackLeft == 0)
        {
            mIsStillPiece = false;
        }

        if(!mIsStillPiece)
        {
            endGame = true;
            Canvas tmp = GameObject.FindGameObjectsWithTag("endGamePanel")[0].GetComponent<Canvas>();
            tmp.enabled = true;
            TMP_Text tmp2 = GameObject.Find("endGameMsg").GetComponent<TMP_Text>();
            if(BlackLeft == 0)
            {
                tmp2.text = setLanguage.LMan.getString("congrats"); 
            }
            else
            {
                tmp2.text = setLanguage.LMan.getString("looser"); 
            }
        }

        blackTurn = false;
        bool isBlackTurn = color == Color.white ? true : false;

        if(isBlackTurn == true)
        {
            blackTurn = true;
        }
        SetInteractive(mWhitePieces, !isBlackTurn);
        SetInteractive(mBlackPieces, isBlackTurn);

        foreach(BasePiece piece in mPromotedPieces)
        {
            bool isBlackPiece = piece.mColor != Color.white ? true : false;
            bool isPartOfTeam = isBlackPiece == true ? isBlackTurn : !isBlackTurn;

            piece.enabled = isPartOfTeam;
        }

    }

    public void ResetPieces()
    {
        foreach (BasePiece piece in mPromotedPieces)
        {
            piece.Kill();
            Destroy(piece.gameObject);
        }

        foreach (BasePiece piece in mWhitePieces)
            piece.Reset();

        foreach (BasePiece piece in mBlackPieces)
            piece.Reset();
    }

    public void PromotePiece(Pawn pawn, Cell cell, Color teamColor, Color spriteColor)
    {
        pawn.Kill();

        BasePiece promotedPiece = CreatePiece(typeof(Queen));
        promotedPiece.Setup(teamColor, spriteColor, this);

        promotedPiece.Place(cell);

        mPromotedPieces.Add(promotedPiece);
    }

    public void DeactivateAll(BasePiece activ)
    {
        Deact(mWhitePieces, mBlackPieces, activ);
    }

    public void Deact(List<BasePiece> allWhite,List<BasePiece> allBlack, BasePiece activ)
    {

        foreach (BasePiece piece in allWhite)
            if(piece != activ)
            {
                piece.enabled = false;
            }

        foreach (BasePiece piece in allBlack)
            if (piece != activ)
            {
                piece.enabled = false;
            }
    }
}
