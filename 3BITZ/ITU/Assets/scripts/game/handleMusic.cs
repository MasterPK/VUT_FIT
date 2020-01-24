using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class handleMusic : MonoBehaviour
{
    public Button tlacitko;
    public mute_sound mute_Sound;
    // Start is called before the first frame update
    void Start()
    {
        if (mute_sound.zvuk.isPlaying)
        {
            tlacitko.image.sprite = Resources.Load<Sprite>("speaker");
        }
        else
        {
            tlacitko.image.sprite = Resources.Load<Sprite>("audio-tool-in-silence");
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void MuteClick()
    {
        
        if (mute_sound.zvuk.isPlaying)
        {
            mute_sound.zvuk.Stop();
            tlacitko.image.sprite = Resources.Load<Sprite>("audio-tool-in-silence");
        }
        else
        {
            mute_sound.zvuk.Play();
            tlacitko.image.sprite = Resources.Load<Sprite>("speaker");
        }
    }
}
