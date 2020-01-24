using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AppSettingsMenu : MonoBehaviour
{
    public bool clicked = true;
    public int move_up = -1;
    public int move_down = -1;
    // Start is called before the first frame update
    void Start()
    {



    }


    public void SetProps(int obj_num, bool enable)
    {
        GameObject.Find("Button" + obj_num).GetComponent<Button>().enabled = enable;
        GameObject.Find("Button" + obj_num).GetComponent<Button>().image.enabled = enable;
        GameObject.Find("Text" + obj_num).GetComponent<Text>().enabled = enable;
    }

    public void Move(int button_num, bool direction)
    {
        if (direction) GameObject.Find("Button" + button_num).GetComponent<Button>().transform.Translate(new Vector3(0f, 10f, 0f));
        else GameObject.Find("Button" + button_num).GetComponent<Button>().transform.Translate(new Vector3(0f, -10f, 0f));
    }

    public void onClick()
    {
        if (clicked)
        {
            if (move_up == -1)
            {
                move_up = 0;
                StartRotation(true);
            }
        }
        else
        {
            if (move_down == -1)
            {
                move_down = 0;
                SetProps(8, true);
                StartRotation(false);
            }

        }

    }

    // Update is called once per frame
    void Update()
    {
        if (move_down >= 0)
        {

            if (move_down >= 0 && move_down <= 100)
                Move(8, false);

            if (move_down >= 50 && move_down <= 100)
                Move(7, false);

            move_down += 10;

            if(move_down == 50)
            {
                SetProps(7, true);
            }

            if (move_down == 100)
            {
                move_down = -1;
                clicked = true;
            }
        }

        if (move_up >= 0)
        {

            if (move_up >= 0 && move_up < 100)
                Move(8, true);

            if (move_up >= 0 && move_up < 50)
                Move(7, true);

            move_up += 10;

            if (move_up == 50)
            {
                SetProps(7, false);
            }

            if (move_up == 100)
            {
                SetProps(8, false);
                move_up = -1;
                clicked = false;
            }
        }
    }

    private bool rotating;

    private IEnumerator Rotate(Vector3 angles, float duration)
    {
        rotating = true;
        Quaternion startRotation = GameObject.Find("Image2").GetComponent<Image>().transform.rotation;
        Quaternion endRotation = Quaternion.Euler(angles) * startRotation;
        for (float t = 0; t < duration; t += Time.deltaTime)
        {
            GameObject.Find("Image2").GetComponent<Image>().transform.rotation = Quaternion.Lerp(startRotation, endRotation, t / duration);
            yield return null;
        }
        GameObject.Find("Image2").GetComponent<Image>().transform.rotation = endRotation;
        rotating = false;
    }

    public void StartRotation(bool direction)
    {
        if (!rotating)
        {
            if (direction) StartCoroutine(Rotate(new Vector3(0, 0, -180), 0.1f));
            else StartCoroutine(Rotate(new Vector3(0, 0, 180), 0.1f));
        }

    }
}
