# Unity Preflight Collision Detection & Cleanup
# Purpose: Detect and terminate stale Unity processes before batch-mode execution
# Usage: powershell -ExecutionPolicy Bypass -File scripts/unity-preflight.ps1
#        (Called automatically from CI jobs)

param(
    [switch]$Force = $false,
    [switch]$Verbose = $false
)

$ErrorActionPreference = "Stop"

function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $output = "[$timestamp] [$Level] $Message"
    Write-Host $output
    return $output
}

Write-Log "=== Unity Preflight Collision Detection ===" "START"

# ============================================================================
# 1. Detect existing Unity processes
# ============================================================================
Write-Log "Scanning for existing Unity processes..."
$processNames = @("Unity", "UnityHub", "Unity.LicenseClient", "UnityShaderCompiler")
$unityProcesses = @()

foreach ($name in $processNames) {
    $procs = Get-Process -Name $name -ErrorAction SilentlyContinue
    if ($procs) {
        $unityProcesses += $procs
    }
}

if ($unityProcesses.Count -eq 0) {
    Write-Log "✅ No existing Unity processes found" "SUCCESS"
    exit 0
}

Write-Log "⚠️  Found $($unityProcesses.Count) existing Unity processes:" "WARNING"
foreach ($proc in $unityProcesses) {
    Write-Log "  - $($proc.ProcessName) (PID: $($proc.Id), Memory: $([math]::Round($proc.WorkingSet / 1MB))MB)" "WARNING"
}

# ============================================================================
# 2. Check for lock files in Unity project directories
# ============================================================================
Write-Log "Checking for Unity lock files..."
$lockFiles = @(
    "native-frontends/unity-frontend/Library/lock",
    "native-frontends/unity-frontend/Library/EditorInstance.json"
)

foreach ($file in $lockFiles) {
    if (Test-Path $file) {
        Write-Log "  Found lock file: $file" "WARNING"
    }
}

# ============================================================================
# 3. Terminate processes (with graceful shutdown attempts)
# ============================================================================
if ($Force) {
    Write-Log "Proceeding with force termination (--Force flag set)" "INFO"
} else {
    Write-Log "Attempting graceful shutdown (CloseMainWindow)..." "INFO"
}

$failedTerminations = @()
foreach ($proc in $unityProcesses) {
    try {
        if (-not $Force) {
            # Try graceful shutdown first
            Write-Log "Sending close signal to $($proc.ProcessName) (PID: $($proc.Id))..." "INFO"
            $proc.CloseMainWindow() | Out-Null
            Start-Sleep -Milliseconds 500
            
            # Check if process actually closed
            $proc.Refresh()
            if ($proc.HasExited) {
                Write-Log "✓ $($proc.ProcessName) (PID: $($proc.Id)) closed gracefully" "SUCCESS"
                continue
            }
        }
        
        # Force kill if graceful shutdown failed or --Force is set
        Write-Log "Force terminating $($proc.ProcessName) (PID: $($proc.Id))..." "INFO"
        Stop-Process -Id $proc.Id -Force
        Start-Sleep -Milliseconds 250
        
        Write-Log "✓ Forcefully terminated $($proc.ProcessName) (PID: $($proc.Id))" "SUCCESS"
    } catch {
        Write-Log "❌ Failed to terminate $($proc.ProcessName) (PID: $($proc.Id)): $_" "ERROR"
        $failedTerminations += $proc
    }
}

# ============================================================================
# 4. Verify cleanup
# ============================================================================
Write-Log "Verifying cleanup..."
Start-Sleep -Milliseconds 1000

$remaining = @()
foreach ($name in $processNames) {
    $procs = Get-Process -Name $name -ErrorAction SilentlyContinue
    if ($procs) {
        $remaining += $procs
    }
}

if ($remaining.Count -gt 0) {
    Write-Log "❌ ERROR: $($remaining.Count) Unity process(es) still running after cleanup:" "ERROR"
    foreach ($proc in $remaining) {
        Write-Log "  - $($proc.ProcessName) (PID: $($proc.Id))" "ERROR"
    }
    exit 1
}

Write-Log "✅ Cleanup verified: All Unity processes terminated successfully" "SUCCESS"

# ============================================================================
# 5. Clean up lock files
# ============================================================================
Write-Log "Cleaning up lock files..."
foreach ($file in $lockFiles) {
    if (Test-Path $file) {
        try {
            Remove-Item -Path $file -Force -ErrorAction SilentlyContinue
            Write-Log "✓ Removed lock file: $file" "SUCCESS"
        } catch {
            Write-Log "⚠️  Could not remove lock file $file (may be in use): $_" "WARNING"
        }
    }
}

# ============================================================================
# 6. Clear editor preferences/temp files (optional, disabled by default)
# ============================================================================
if ($Force) {
    Write-Log "Force flag set: Clearing additional temporary files..." "INFO"
    $tempDirs = @(
        "native-frontends/unity-frontend/Library/EditorInstance.json",
        "native-frontends/unity-frontend/Temp"
    )
    foreach ($dir in $tempDirs) {
        if (Test-Path $dir) {
            try {
                if (Test-Path $dir -PathType Container) {
                    Remove-Item -Path $dir -Recurse -Force -ErrorAction SilentlyContinue
                } else {
                    Remove-Item -Path $dir -Force -ErrorAction SilentlyContinue
                }
                Write-Log "✓ Cleared: $dir" "SUCCESS"
            } catch {
                Write-Log "⚠️  Could not clear $dir: $_" "WARNING"
            }
        }
    }
}

Write-Log "=== Unity Preflight Complete ===" "SUCCESS"
exit 0
