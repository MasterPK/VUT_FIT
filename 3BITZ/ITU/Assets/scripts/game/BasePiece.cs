using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections.Generic;

public abstract class BasePiece : EventTrigger
{
    [HideInInspector]
    public Color mColor = Color.clear;
    public bool mIsFirstMove = true;

    protected Cell mOriginalCell = null;
    protected Cell mCurrentCell = null;

    protected RectTransform mRectTransform = null;
    protected PieceManager mPieceManager;

    protected Cell mTargetCell = null;
    private bool Vyhodenie = false;
    private bool next_jump = false;
    public List<BasePiece> pieces = new List<BasePiece>();

    protected Vector3Int mMovement = Vector3Int.one;
    protected List<Cell> mHighlightedCells = new List<Cell>();
    protected List<Cell> mDropOutCells = new List<Cell>();

    public void Update()
    {
        if(mPieceManager.blackTurn == true && mPieceManager.endGame == false && SettingsHandler.multiplayer == false)
        {
            foreach (BasePiece piece in mPieceManager.mBlackPieces)
            {
                CheckPathing();
                if (mHighlightedCells.Count > 0)
                {
                    pieces.Add(piece);
                }
                ClearCells();
                ClearCellsDrop();
                mHighlightedCells.Clear();

            }
            int choose = Random.Range(0, pieces.Count-1);
            BasePiece chosenOne = pieces[choose];
            pieces.Clear();
            pieces.Add(chosenOne);
            foreach(BasePiece piece in pieces)
            {
                CheckPathing();
                int tmp = Random.Range(0, mHighlightedCells.Count - 1);

                mTargetCell = mHighlightedCells[tmp];

                if (mColor == Color.white && mTargetCell != null)
                {
                    if ((mTargetCell.mBoardPosition.y - mCurrentCell.mBoardPosition.y) == 2)
                    {
                        int x = mCurrentCell.mBoardPosition.x;
                        int y = mCurrentCell.mBoardPosition.y;
                        y += 1;
                        if (mCurrentCell.mBoardPosition.x < mTargetCell.mBoardPosition.x)
                        {
                            x += 1;
                        }
                        else
                        {
                            x -= 1;
                        }
                        Cell destCell = mTargetCell.mBoard.mAllCells[x, y];
                        destCell.RemovePiece();
                        mPieceManager.BlackLeft -= 1;
                    }

                    if ((mTargetCell.mBoardPosition.y - mCurrentCell.mBoardPosition.y) == -2)
                    {
                        int x = mCurrentCell.mBoardPosition.x;
                        int y = mCurrentCell.mBoardPosition.y;
                        y -= 1;
                        if (mCurrentCell.mBoardPosition.x < mTargetCell.mBoardPosition.x)
                        {
                            x += 1;
                        }
                        else
                        {
                            x -= 1;
                        }
                        Cell destCell = mTargetCell.mBoard.mAllCells[x, y];
                        destCell.RemovePiece();
                        mPieceManager.BlackLeft -= 1;
                    }
                }

                if (mColor == Color.black && mTargetCell != null)
                {
                    if ((mCurrentCell.mBoardPosition.y - mTargetCell.mBoardPosition.y) == 2)
                    {
                        int x = mCurrentCell.mBoardPosition.x;
                        int y = mCurrentCell.mBoardPosition.y;
                        y -= 1;
                        if (mCurrentCell.mBoardPosition.x < mTargetCell.mBoardPosition.x)
                        {
                            x += 1;
                        }
                        else
                        {
                            x -= 1;
                        }
                        Cell destCell = mTargetCell.mBoard.mAllCells[x, y];
                        destCell.RemovePiece();
                        mPieceManager.WhiteLeft -= 1;
                    }

                    if ((mCurrentCell.mBoardPosition.y - mTargetCell.mBoardPosition.y) == -2)
                    {
                        int x = mCurrentCell.mBoardPosition.x;
                        int y = mCurrentCell.mBoardPosition.y;
                        y += 1;
                        if (mCurrentCell.mBoardPosition.x < mTargetCell.mBoardPosition.x)
                        {
                            x += 1;
                        }
                        else
                        {
                            x -= 1;
                        }
                        Cell destCell = mTargetCell.mBoard.mAllCells[x, y];
                        destCell.RemovePiece();
                        mPieceManager.WhiteLeft -= 1;
                    }
                }

                ClearCells();
                ClearCellsDrop();
                mHighlightedCells.Clear();


                Move();

                mPieceManager.SwitchSides(mColor);

            }



        }
    }
    public virtual void Setup(Color newTeamColor, Color32 newSpriteColor, PieceManager newPieceManager)
    {
        mPieceManager = newPieceManager;

        mColor = newTeamColor;
        GetComponent<Image>().color = newSpriteColor;
        mRectTransform = GetComponent<RectTransform>();
    }

    public virtual void Place(Cell newCell)
    {
        mCurrentCell = newCell;
        mOriginalCell = newCell;
        mCurrentCell.mCurrentPiece = this;

        transform.position = newCell.transform.position;
        gameObject.SetActive(true);
    }

    public virtual void Reset()
    {
        Kill();

        Place(mOriginalCell);
    }

    public virtual void Kill()
    {
        mCurrentCell.mCurrentPiece = null;

        gameObject.SetActive(false);
    }

    #region Movement
    private void CreateCellPath(int xDirection, int yDirection, int movement)
    {
        int currentX = mCurrentCell.mBoardPosition.x;
        int currentY = mCurrentCell.mBoardPosition.y;

        for (int i = 1; i <= movement; i++)
        {
            currentX += xDirection;
            currentY += yDirection;

            CellState cellState = CellState.None;
            cellState = mCurrentCell.mBoard.ValidateCell(currentX, currentY, this);

            if(cellState == CellState.Enemy)
            {
                mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[currentX, currentY]);
                break;
            }

            if (cellState != CellState.Free)
                break;

            mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[currentX, currentY]);
        }
    }

    protected virtual void CheckPathing()
    {

        CreateCellPath(1, 0, mMovement.x);
        CreateCellPath(-1, 0, mMovement.x);

        CreateCellPath(0, 1, mMovement.y);
        CreateCellPath(0, -1, mMovement.y);

        CreateCellPath(1, 1, mMovement.z);
        CreateCellPath(-1, 1, mMovement.z);

        CreateCellPath(1, -1, mMovement.z);
        CreateCellPath(-1, -1, mMovement.z);
    }

    protected void ShowCells()
    {
        foreach (Cell cell in mHighlightedCells)
            cell.mOutlineImage.enabled = true;
    }

    protected void ShowDropoutCells()
    {
        foreach (Cell cell in mDropOutCells)
            cell.mOutlineImage.enabled = true;
    }

    protected void ClearCellsDrop()
    {
        foreach (Cell cell in mDropOutCells)
            cell.mOutlineImage.enabled = false;
    }

    protected void ClearCells()
    {
        foreach (Cell cell in mHighlightedCells)
            cell.mOutlineImage.enabled = false;
    }

    protected void CheckWhite()
    {
        if (mPieceManager.blackTurn == false)
        {
            foreach (BasePiece piece in mPieceManager.mWhitePieces)
            {
                CheckPathing();
            }
            if(mHighlightedCells.Count == 0)
            {
                mPieceManager.Deadlock();
            }
        }
    }

    protected virtual void Move()
    {
        mTargetCell.RemovePiece();

        mCurrentCell.mCurrentPiece = null;

        mCurrentCell = mTargetCell;
        mCurrentCell.mCurrentPiece = this;

        transform.position = mCurrentCell.transform.position;
        mTargetCell = null;
    }
    #endregion

    #region Events
    public override void OnBeginDrag(PointerEventData eventData)
    {
        base.OnBeginDrag(eventData);

        if(next_jump == false)
        {
            CheckPathing();
            ShowCells();
        }
        else
        {
            ShowDropoutCells();
        }
    }

    public override void OnDrag(PointerEventData eventData)
    {
        base.OnDrag(eventData);

        transform.position += (Vector3)eventData.delta;

        if(next_jump == false)
        {
            foreach (Cell cell in mHighlightedCells)
            {
                if (RectTransformUtility.RectangleContainsScreenPoint(cell.mRectTransform, Input.mousePosition))
                {
                    mTargetCell = cell;
                    break;
                }
                mTargetCell = null;
            }
        }
        else
        {
            foreach (Cell cell in mDropOutCells)
            {
                if (RectTransformUtility.RectangleContainsScreenPoint(cell.mRectTransform, Input.mousePosition))
                {
                    mTargetCell = cell;
                    break;
                }
                mTargetCell = null;
            }
        }



    }

    public override void OnEndDrag(PointerEventData eventData)
    {
        base.OnEndDrag(eventData);

        Vyhodenie = false;
        if (mColor == Color.white && mTargetCell != null)
        {
            if((mTargetCell.mBoardPosition.y - mCurrentCell.mBoardPosition.y) == 2)
            {
                int x = mCurrentCell.mBoardPosition.x;
                int y = mCurrentCell.mBoardPosition.y;
                y += 1;
                if(mCurrentCell.mBoardPosition.x < mTargetCell.mBoardPosition.x)
                {
                    x += 1;
                }
                else
                {
                    x -= 1;
                }
                Cell destCell = mTargetCell.mBoard.mAllCells[x, y];
                destCell.RemovePiece();
                Vyhodenie = true;
                mPieceManager.BlackLeft -= 1;
            }

            if ((mTargetCell.mBoardPosition.y - mCurrentCell.mBoardPosition.y) == -2)
            {
                int x = mCurrentCell.mBoardPosition.x;
                int y = mCurrentCell.mBoardPosition.y;
                y -= 1;
                if (mCurrentCell.mBoardPosition.x < mTargetCell.mBoardPosition.x)
                {
                    x += 1;
                }
                else
                {
                    x -= 1;
                }
                Cell destCell = mTargetCell.mBoard.mAllCells[x, y];
                destCell.RemovePiece();
                Vyhodenie = true;
                mPieceManager.BlackLeft -= 1;
            }
        }

        if (mColor == Color.black && mTargetCell != null)
        {
            if ((mCurrentCell.mBoardPosition.y - mTargetCell.mBoardPosition.y) == 2)
            {
                int x = mCurrentCell.mBoardPosition.x;
                int y = mCurrentCell.mBoardPosition.y;
                y -= 1;
                if (mCurrentCell.mBoardPosition.x < mTargetCell.mBoardPosition.x)
                {
                    x += 1;
                }
                else
                {
                    x -= 1;
                }
                Cell destCell = mTargetCell.mBoard.mAllCells[x, y];
                destCell.RemovePiece();
                Vyhodenie = true;
                mPieceManager.WhiteLeft -= 1;
            }

            if ((mCurrentCell.mBoardPosition.y - mTargetCell.mBoardPosition.y) == -2)
            {
                int x = mCurrentCell.mBoardPosition.x;
                int y = mCurrentCell.mBoardPosition.y;
                y += 1;
                if (mCurrentCell.mBoardPosition.x < mTargetCell.mBoardPosition.x)
                {
                    x += 1;
                }
                else
                {
                    x -= 1;
                }
                Cell destCell = mTargetCell.mBoard.mAllCells[x, y];
                destCell.RemovePiece();
                Vyhodenie = true;
                mPieceManager.WhiteLeft -= 1;
            }
        }

        ClearCells();
        ClearCellsDrop();
        mHighlightedCells.Clear();

        if (!mTargetCell)
        {
            transform.position = mCurrentCell.gameObject.transform.position;
            return;
        }

        mDropOutCells.Clear();


        Move();

        
        if(Vyhodenie == true && (mCurrentCell.mCurrentPiece.GetType() == typeof(Queen)))
        {
            int curentX = mCurrentCell.mBoardPosition.x;
            int curentY = mCurrentCell.mBoardPosition.y;

            CellState cellState = CellState.None;
            CellState cellState2 = CellState.None;
            CellState cellState3 = CellState.None;
            CellState cellState4 = CellState.None;
            CellState cellStatetmp = CellState.None;

            int col = 0;

            cellState = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z, curentY + mMovement.z, this);
            cellState2 = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z, curentY + mMovement.z, this);
            cellState3 = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z, curentY - mMovement.z, this);
            cellState4 = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z, curentY - mMovement.z, this);


            if (cellState == CellState.Enemy)
            {
                col = 1;

                if (col == 1 && mColor == Color.white)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z + mMovement.z, curentY + mMovement.z + mMovement.z, this);
                    if (cellStatetmp == CellState.Free)
                    {
                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX + mMovement.z + mMovement.z, curentY + mMovement.z + mMovement.z]);
                    }
                }

                if (col == 1 && mColor == Color.black)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z + mMovement.z, curentY + mMovement.z + mMovement.z, this);
                    if (cellStatetmp == CellState.Free)
                    {
                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX + mMovement.z + mMovement.z, curentY + mMovement.z + mMovement.z]);
                    }
                }
            }

            if (cellState2 == CellState.Enemy)
            {
                col = 2;

                if (col == 2 && mColor == Color.white)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z - mMovement.z, curentY + mMovement.z + mMovement.z, this);

                    if (cellStatetmp == CellState.Free)
                    {

                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX - mMovement.z - mMovement.z, curentY + mMovement.z + mMovement.z]);
                    }
                }

                if (col == 2 && mColor == Color.black)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z - mMovement.z, curentY + mMovement.z + mMovement.z, this);

                    if (cellStatetmp == CellState.Free)
                    {

                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX - mMovement.z - mMovement.z, curentY + mMovement.z + mMovement.z]);
                    }
                }

            }

            if (cellState3 == CellState.Enemy)
            {
                col = 3;

                if (col == 2 && mColor == Color.white)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z + mMovement.z, curentY - mMovement.z - mMovement.z, this);

                    if (cellStatetmp == CellState.Free)
                    {

                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX + mMovement.z + mMovement.z, curentY - mMovement.z - mMovement.z]);
                    }
                }

                if (col == 2 && mColor == Color.black)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z + mMovement.z, curentY - mMovement.z - mMovement.z, this);

                    if (cellStatetmp == CellState.Free)
                    {

                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX + mMovement.z + mMovement.z, curentY - mMovement.z - mMovement.z]);
                    }
                }

            }

            if (cellState4 == CellState.Enemy)
            {
                col = 4;

                if (col == 2 && mColor == Color.white)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z - mMovement.z, curentY - mMovement.z - mMovement.z, this);

                    if (cellStatetmp == CellState.Free)
                    {

                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX - mMovement.z - mMovement.z, curentY - mMovement.z - mMovement.z]);
                    }
                }

                if (col == 2 && mColor == Color.black)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z - mMovement.z, curentY - mMovement.z - mMovement.z, this);

                    if (cellStatetmp == CellState.Free)
                    {

                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX - mMovement.z - mMovement.z, curentY - mMovement.z - mMovement.z]);
                    }
                }

            }
        }

        if (Vyhodenie == true && (mCurrentCell.mCurrentPiece.GetType() == typeof(Pawn)))
        {
            int curentX = mCurrentCell.mBoardPosition.x;
            int curentY = mCurrentCell.mBoardPosition.y;

            CellState cellState = CellState.None;
            CellState cellState2 = CellState.None;
            CellState cellStatetmp = CellState.None;

            int col = 0;

            cellState = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z, curentY + mMovement.z, this);
            cellState2 = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z, curentY + mMovement.z, this);


            if (cellState == CellState.Enemy)
            {
                col = 1;

                if (col == 1 && mColor == Color.white)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z + mMovement.z, curentY + mMovement.z + mMovement.z, this);
                    if (cellStatetmp == CellState.Free)
                    {
                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX + mMovement.z + mMovement.z, curentY + mMovement.z + mMovement.z]);
                    }
                }

                if (col == 1 && mColor == Color.black)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z + mMovement.z, curentY + mMovement.z + mMovement.z, this);
                    if (cellStatetmp == CellState.Free)
                    {
                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX + mMovement.z + mMovement.z, curentY + mMovement.z + mMovement.z]);
                    }
                }
            }

            if(cellState2 == CellState.Enemy)
            {
                col = 2;

                if (col == 2 && mColor == Color.white)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z - mMovement.z, curentY + mMovement.z + mMovement.z, this);

                    if (cellStatetmp == CellState.Free)
                    {

                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX - mMovement.z - mMovement.z, curentY + mMovement.z + mMovement.z]);
                    }
                }

                if (col == 2 && mColor == Color.black)
                {
                    cellStatetmp = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z - mMovement.z, curentY + mMovement.z + mMovement.z, this);

                    if (cellStatetmp == CellState.Free)
                    {

                        mDropOutCells.Add(mCurrentCell.mBoard.mAllCells[curentX - mMovement.z - mMovement.z, curentY + mMovement.z + mMovement.z]);
                    }
                }

            }

        }
        if(mDropOutCells.Count > 0)
        {
            mPieceManager.DeactivateAll(mCurrentCell.mCurrentPiece);
            next_jump = true;
        }
        else
        {
            next_jump = false;
            mPieceManager.SwitchSides(mColor);
        }

    }
    #endregion
}
