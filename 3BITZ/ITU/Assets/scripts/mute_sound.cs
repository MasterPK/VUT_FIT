//Petr Křehlík (xkrehl04)
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class mute_sound : MonoBehaviour
{
    public TextMeshProUGUI txt;
    public Button tlacitko;
    public static AudioSource zvuk;
    public AudioSource sound;
    // Start is called before the first frame update
    void Start()
    {
        if(sound!=null && zvuk==null)
        {
            zvuk = sound;
        }
        else
        {
            //GameObject[] objs = GameObject.FindGameObjectsWithTag("music");
            //zvuk = objs[0].GetComponent<AudioSource>();
        }

        if (zvuk.isPlaying)
        {
            tlacitko.image.sprite = Resources.Load<Sprite>("speaker");
        }
        else
        {
            tlacitko.image.sprite = Resources.Load<Sprite>("audio-tool-in-silence");
        }

        tlacitko.onClick.AddListener(TaskOnClick);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void TaskOnClick()
    {
        if (zvuk.isPlaying)
        {
            zvuk.Stop();
            tlacitko.image.sprite= Resources.Load<Sprite>("audio-tool-in-silence");
        }
        else
        {
            zvuk.Play();
            tlacitko.image.sprite = Resources.Load<Sprite>("speaker");
        }
    }
}
