using UnityEngine;
using UnityEngine.UIElements;
using UnityEngine.Audio;

public class ComplianceDispute : MonoBehaviour
{
    [SerializeField] private AudioClip alarmChime;
    [SerializeField] private AudioClip successChime;
    [SerializeField] private AudioMixerGroup alarmMixer;
    [SerializeField] private AudioMixerGroup musicMixer;
    
    private VisualElement root;
    private Label countdownLabel;
    private Label stepNameLabel;
    private Label statusLabel;
    private Button copyHashButton;
    private TextField provenanceHashField;
    private VisualElement escalationPulse;
    private VisualElement hashTooltip;
    private AudioSource audioSource;
    
    private System.TimeSpan slaRemaining;
    private bool bIsEscalated = false;
    private string currentProvenanceHash = "";
    private double elapsedTime = 0;
    
    private void OnEnable()
    {
        root = GetComponent<UIDocument>().rootVisualElement;
        
        // Cache UI elements
        countdownLabel = root.Q<Label>("CountdownLabel");
        stepNameLabel = root.Q<Label>("StepNameLabel");
        statusLabel = root.Q<Label>("StatusLabel");
        copyHashButton = root.Q<Button>("CopyHashButton");
        provenanceHashField = root.Q<TextField>("ProvenanceHash");
        escalationPulse = root.Q<VisualElement>("EscalationPulse");
        hashTooltip = root.Q<VisualElement>("HashTooltip");
        
        // Setup audio source
        audioSource = gameObject.AddComponent<AudioSource>();
        audioSource.outputAudioMixerGroup = alarmMixer;
        
        // Setup copy button callback
        copyHashButton.clicked += CopyHashToClipboard;
        
        // Setup tooltip hover
        provenanceHashField.RegisterCallback<MouseEnterEvent>(ShowTooltip);
        provenanceHashField.RegisterCallback<MouseLeaveEvent>(HideTooltip);
    }
    
    private void OnDisable()
    {
        copyHashButton?.UnregisterCallback<ClickEvent>(evt => CopyHashToClipboard());
    }
    
    private void Update()
    {
        elapsedTime += Time.deltaTime;
        
        // Update countdown timer (HH:MM:SS format)
        if (slaRemaining.TotalSeconds > 0)
        {
            slaRemaining = slaRemaining.Subtract(System.TimeSpan.FromSeconds(Time.deltaTime));
            
            int hours = (int)slaRemaining.TotalHours;
            int minutes = slaRemaining.Minutes;
            int seconds = slaRemaining.Seconds;
            
            countdownLabel.text = $"{hours:D2}:{minutes:D2}:{seconds:D2}";
            
            // Trigger escalation if SLA exceeded
            if (slaRemaining.TotalSeconds <= 0 && !bIsEscalated)
            {
                OnEscalation();
            }
        }
    }
    
    public void SetDisputeStep(string stepName, bool bStepComplete)
    {
        stepNameLabel.text = stepName;
        stepNameLabel.AddToClassList("glow-animation");
        
        if (bStepComplete)
        {
            PlayChimeAudio();
            statusLabel.text = "Complete";
            statusLabel.style.color = new Color(0.5f, 0.8f, 0.5f, 1.0f);
        }
    }
    
    public void SetSLARemaining(double totalSeconds)
    {
        slaRemaining = System.TimeSpan.FromSeconds(totalSeconds);
    }
    
    public void SetProvenanceHash(string hashValue)
    {
        currentProvenanceHash = hashValue;
        provenanceHashField.value = hashValue.Substring(0, Mathf.Min(16, hashValue.Length)) + "...";
    }
    
    public void OnEscalation()
    {
        bIsEscalated = true;
        escalationPulse.AddToClassList("active");
        PlayAlarmAudio();
        statusLabel.text = "Escalated";
        statusLabel.style.color = new Color(1.0f, 0.0f, 0.0f, 1.0f);
    }
    
    public void OnResolution()
    {
        bIsEscalated = false;
        escalationPulse.RemoveFromClassList("active");
        PlayChimeAudio();
        statusLabel.text = "Resolved";
        statusLabel.style.color = new Color(0.5f, 0.8f, 0.5f, 1.0f);
    }
    
    private void PlayAlarmAudio()
    {
        if (alarmChime != null && audioSource != null)
        {
            audioSource.clip = alarmChime;
            audioSource.volume = 1.0f;
            audioSource.Play();
            
            // Apply sidechain ducking (reduce music to 0.3f)
            if (musicMixer != null)
            {
                StartCoroutine(DuckMusicVolume(true));
            }
        }
    }
    
    private void PlayChimeAudio()
    {
        if (successChime != null && audioSource != null)
        {
            audioSource.clip = successChime;
            audioSource.volume = 0.8f;
            audioSource.Play();
        }
    }
    
    private void CopyHashToClipboard()
    {
        GUIUtility.systemCopyBuffer = currentProvenanceHash;
        Debug.Log($"[ComplianceDispute] Hash copied to clipboard: {currentProvenanceHash.Substring(0, 16)}...");
    }
    
    private void ShowTooltip(MouseEnterEvent evt)
    {
        hashTooltip.AddToClassList("visible");
        hashTooltip.Q<Label>().text = $"SHA256: {currentProvenanceHash}";
    }
    
    private void HideTooltip(MouseLeaveEvent evt)
    {
        hashTooltip.RemoveFromClassList("visible");
    }
    
    private System.Collections.IEnumerator DuckMusicVolume(bool bDuck)
    {
        float targetVolume = bDuck ? 0.3f : 1.0f;
        float duration = 0.5f;
        float elapsed = 0;
        
        // Note: In production, use AudioMixer.SetFloat for proper bus ducking
        // This is a simplified version for demonstration
        
        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            yield return null;
        }
    }
}
